#include "YSFIOKernel.h"
/*************************************************************************************
 *
 * 文 件 名:	YSFIOKernel.cpp
 * 描    述:	内核实现文件
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/9/29 9:08
 *
*************************************************************************************/

#include <iostream>
#include <string>
#include <algorithm>
#include <unistd.h>
#include <sys/epoll.h>
#include "util.h"
#include "AYSFIOHandle.h"
#include "IYSFIOChannel.h"
#include "IYSFIOProtocol.h"
#include "IYSFIORole.h"
#include "YSFIOStreamMsg.h"

namespace YSFIO
{
	using namespace std;
	std::unique_ptr<YSFIOKernel> YSFIOKernel::kernel = nullptr;

	void YSFIOKernel::SetChannelOut(IYSFIOChannel* _channel)
	{
		epoll_event evn;
		evn.events = EPOLLIN | EPOLLOUT;
		evn.data.ptr = static_cast<void*>(_channel);
		epoll_ctl(kernel->m_epollFd, EPOLL_CTL_MOD, _channel->GetFd(), &evn);
		return;
	}

	void YSFIOKernel::ClearChannelOut(IYSFIOChannel* _channel)
	{
		epoll_event evn;
		evn.events = EPOLLIN;
		evn.data.ptr = static_cast<void*>(_channel);
		epoll_ctl(kernel->m_epollFd, EPOLL_CTL_MOD, _channel->GetFd(), &evn);
		return;
	}

	bool YSFIOKernel::Init()
	{
		if (nullptr == kernel)
		{
			/* 生成全局单例 */
			kernel.reset(new YSFIOKernel{});
		}
		return kernel->m_Init();
	}

	void YSFIOKernel::Run()
	{
		kernel->m_Run();
	}

	void YSFIOKernel::Fini()
	{
		kernel->m_Fini();
		kernel.reset();
	}

	bool YSFIOKernel::AddChannel(IYSFIOChannel* _channel)
	{
		return kernel->m_AddChannel(_channel);
	}

	void YSFIOKernel::DelChannel(IYSFIOChannel* _channel)
	{
		kernel->m_DelChannel(_channel);
		return;
	}

	bool YSFIOKernel::AddProtocol(IYSFIOProtocol* _protocol)
	{
		return kernel->m_AddProtocol(_protocol);
	}

	void YSFIOKernel::DelProtocol(IYSFIOProtocol* _protocol)
	{
		kernel->m_DelProtocol(_protocol);
		return;
	}

	bool YSFIOKernel::AddRole(IYSFIORole* _role)
	{
		return kernel->m_AddRole(_role);
	}

	void YSFIOKernel::DelRole(IYSFIORole* _role)
	{
		kernel->m_DelRole(_role);
		return;
	}

	void YSFIOKernel::SendOut(UserData& _userData, IYSFIOProtocol* _outProto)
	{
		if (nullptr != _outProto)
		{
			UserMsg msg{ BytesMsg{YSFIOFrameMsg{YSFIOFrameMsg::FrameMsgType::OUT}} };
			msg.userData = &_userData;
			_outProto->Handle(msg);
			msg.userData = nullptr;
		}
	}

	void YSFIOKernel::SendOut(std::string& _output, IYSFIOChannel* _outChannel)
	{
		if (nullptr != _outChannel)
		{
			BytesMsg msg{ YSFIOFrameMsg{YSFIOFrameMsg::FrameMsgType::OUT} };
			msg.bytesData = _output;
			_outChannel->Handle(msg);
		}
	}

	YSFIOKernel::~YSFIOKernel()
	{
		m_Fini();
	}

	void YSFIOKernel::Exit()
	{
		kernel->m_isRun = false;
	}

	std::list<IYSFIOChannel*> YSFIOKernel::GetInfoChannel(const std::string& _info)
	{
		std::list<IYSFIOChannel*> channelList;
		for (auto& channel : kernel->m_lChannel)
		{
			if (channel->GetChannelInfo() == _info)
			{
				channelList.push_back(channel);
			}
		}
		return channelList;
	}

	IYSFIOChannel* YSFIOKernel::GetFirstInfoChannel(const std::string& _info)
	{
		for (auto& channel : kernel->m_lChannel)
		{
			if (channel->GetChannelInfo() == _info)
			{
				return channel;
			}
		}
		return nullptr;
	}

	YSFIOKernel::YSFIOKernel() :
		m_epollFd{ -1 },
		m_isRun{ false }
	{
	}

	bool YSFIOKernel::m_Init()
	{
		bool bRet = false;
		do
		{
			int fd = epoll_create(1);
			if (fd < 0)
			{
				LOG("epoll_create error");
				break;
			}
			m_epollFd = fd;
			m_isRun = true;
			bRet = true;
		} while (false);
		return bRet;
	}

	void YSFIOKernel::m_Run()
	{
		int epollNum = -1;
		while (m_isRun)
		{
			epoll_event evns[100] = { 0 };
			epollNum = epoll_wait(m_epollFd, evns, sizeof(evns) / sizeof(evns[0]), -1);
			if (0 > epollNum)
			{
				if (EINTR == errno)
				{
					/* 信号打断 */
					continue;
				}
				/* 出现错误 */
				LOG("epoll_wait error");
				break;
			}
			else if (0 == epollNum)
			{
				/* 扩展内容，超时处理 */
			}
			else
			{
				/* 有事件触发 */
				for (size_t i = 0; i < epollNum; ++i)
				{
					ProcEvns(evns[i]);
				}
			}
		}
	}

	void YSFIOKernel::m_Fini()
	{
		if (0 < m_epollFd)
		{
			close(m_epollFd);
			m_epollFd = -1;
		}
		for (auto& channel : m_lChannel)
		{
			if (nullptr != channel)
			{
				channel->Fini();
				delete channel;
			}
		}
		for (auto& protoc : m_lProtocol)
		{
			if (nullptr != protoc)
			{
				delete protoc;
			}
		}
		for (auto& role : m_lRole)
		{
			if (nullptr != role)
			{
				role->Fini();
				delete role;
			}
		}
		m_lChannel.clear();
		m_lProtocol.clear();
		m_lRole.clear();
	}

	bool YSFIOKernel::m_AddChannel(IYSFIOChannel* _channel)
	{
		bool bRet = false;
		do
		{
			if (!m_isRun)
			{
				break;
			}
			if (nullptr == _channel || !_channel->Init())
			{
				break;
			}
			epoll_event evn;
			evn.events = EPOLLIN;
			evn.data.ptr = static_cast<void*>(_channel);
			if (0 > epoll_ctl(m_epollFd, EPOLL_CTL_ADD, _channel->GetFd(), &evn))
			{
				break;
			}
			if (std::find(m_lChannel.begin(), m_lChannel.end(), _channel) == m_lChannel.end())
			{
				/* 不存在则添加 */
				m_lChannel.push_back(_channel);
			}
			bRet = true;
		} while (false);
		return bRet;
	}

	void YSFIOKernel::m_DelChannel(IYSFIOChannel* _channel)
	{
		if (!m_isRun)
		{
			return;
		}
		if (nullptr == _channel)
		{
			return;
		}
		if (0 > epoll_ctl(m_epollFd, EPOLL_CTL_DEL, _channel->GetFd(), nullptr))
		{
			return;
		}
		m_lChannel.remove(_channel);
		_channel->Fini();
		delete _channel;
	}

	bool YSFIOKernel::m_AddProtocol(IYSFIOProtocol* _protocol)
	{
		bool bRet = false;
		do
		{
			if (!m_isRun)
			{
				break;
			}
			if (nullptr == _protocol)
			{
				break;
			}
			if (std::find(m_lProtocol.begin(), m_lProtocol.end(), _protocol) == m_lProtocol.end())
			{
				/* 不存在则添加 */
				m_lProtocol.push_back(_protocol);
			}
			bRet = true;
		} while (false);
		return bRet;
	}

	void YSFIOKernel::m_DelProtocol(IYSFIOProtocol* _protocol)
	{
		if (!m_isRun)
		{
			return;
		}
		if (nullptr == _protocol)
		{
			return;
		}
		m_lProtocol.remove(_protocol);
		delete _protocol;
	}

	bool YSFIOKernel::m_AddRole(IYSFIORole* _role)
	{
		bool bRet = false;
		do
		{
			if (!m_isRun)
			{
				break;
			}
			if (nullptr == _role || !_role->Init())
			{
				break;
			}
			if (std::find(m_lRole.begin(), m_lRole.end(), _role) == m_lRole.end())
			{
				/* 不存在则添加 */
				m_lRole.push_back(_role);
			}
			bRet = true;
		} while (false);
		return bRet;
	}

	void YSFIOKernel::m_DelRole(IYSFIORole* _role)
	{
		if (!m_isRun)
		{
			return;
		}
		if (nullptr == _role)
		{
			return;
		}
		m_lRole.remove(_role);
		_role->Fini();
		delete _role;
	}

	void YSFIOKernel::ProcEvns(epoll_event& _evn)
	{
		auto channel = static_cast<IYSFIOChannel*>(_evn.data.ptr);
		if (nullptr == channel)
		{
			/* 出现异常 */
		}
		if (EPOLLIN & _evn.events)
		{
			/* 读事件 */
			YSFIOFrameMsg msg{ YSFIOFrameMsg::FrameMsgType::IN };
			channel->Handle(msg);
			if (channel->IsNeedClose())
			{
				DelChannel(channel);
			}
		}
		if (EPOLLOUT & _evn.events)
		{
			/* 写事件 */
			channel->FlushCache();
			if (!channel->HasCache())
			{
				ClearChannelOut(channel);
			}
		}
	}

}