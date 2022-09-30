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
#include "YSFIOKernel.h"
#include <iostream>
#include <string>
#include <sys/epoll.h>
#include <unistd.h>
#include <algorithm>
#include "util.h"
using namespace std;
using namespace YSFIO;

bool YSFIOKernel::SetChannelOutEvent(IYSFIOChannel& _channel)
{
	epoll_event evn;
	evn.events = EPOLLIN | EPOLLOUT;
	evn.data.ptr = static_cast<void*>(&_channel);
	if (0 > epoll_ctl(pYSFIOKernel->m_epollFd, EPOLL_CTL_MOD, _channel.GetFd(), &evn))
	{
		LOG("epoll_ctl mod error");
		return false;
	}
	return true;
}

bool YSFIOKernel::ClearChannelOutEvent(IYSFIOChannel& _channel)
{
	epoll_event evn;
	evn.events = EPOLLIN;
	evn.data.ptr = static_cast<void*>(&_channel);
	if (0 > epoll_ctl(pYSFIOKernel->m_epollFd, EPOLL_CTL_MOD, _channel.GetFd(), &evn))
	{
		LOG("epoll_ctl mod error");
		return false;
	}
	return true;
}

std::unique_ptr<YSFIOKernel> YSFIOKernel::pYSFIOKernel{ nullptr };
bool YSFIOKernel::YSFIO_Init()
{
	if (nullptr == pYSFIOKernel)
	{
		pYSFIOKernel.reset(new YSFIOKernel{});
	}
	return pYSFIOKernel->Init();
}

void YSFIOKernel::YSFIO_Run()
{
	pYSFIOKernel->Run();
}

bool YSFIOKernel::YSFIO_AddChannel(std::shared_ptr<IYSFIOChannel> _channel)
{
	bool bRet = false;
	if (_channel->Init())
	{
		bRet = pYSFIOKernel->AddChannel(_channel);
	}
	return bRet;
}

void YSFIOKernel::YSFIO_DelChannel(std::shared_ptr<IYSFIOChannel> _channel)
{
	pYSFIOKernel->DelChannel(_channel);
	return;
}

void YSFIOKernel::YSFIO_Fini()
{
	pYSFIOKernel->Fini();
	pYSFIOKernel.reset(nullptr);
}

void YSFIOKernel::YSFIO_SendOut(std::string& _output, std::shared_ptr<IYSFIOChannel> _channel)
{
	BytesMsg msg{ SysIoReadyMsg{SysIoReadyMsg::OUT} };
	msg.msgData = _output;
	_channel->Handle(msg);
}

YSFIOKernel::YSFIOKernel() :
	m_epollFd{ -1 },
	m_lChannel{}
{
}

YSFIOKernel::~YSFIOKernel()
{
	if (0 <= m_epollFd)
	{
		Fini();
	}
}

void YSFIOKernel::Run()
{
	if (0 > m_epollFd)
	{
		LOG("需调用init初始化");
		return;
	}
	int evnNum = -1;
	while (true)
	{
		epoll_event evns[128] = { 0 };
		evnNum = epoll_wait(m_epollFd, evns, sizeof(evns) / sizeof(evns[0]), -1);
		if (0 > evnNum)
		{
			/* 信号中断 */
			if (EINTR == errno)
			{
				continue;
			}
			break;
		}
		else if (0 == evnNum)
		{
			/* 超时扩展 */
		}
		else
		{
			/* 事件产生 */
			for (size_t i = 0; i < evnNum; ++i)
			{
				if (EPOLLIN & evns[i].events)
				{
					/* 读事件产生 */
					SysIoReadyMsg msg{ SysIoReadyMsg::IN };
					auto channel = static_cast<IYSFIOChannel*>(evns[i].data.ptr);
					if (nullptr == channel)
					{
						break;
					}
					channel->Handle(msg);
				}
				if (EPOLLOUT & evns[i].events)
				{
					/* 写事件产生 */
					auto channel = static_cast<IYSFIOChannel*>(evns[i].data.ptr);
					if (nullptr == channel)
					{
						break;
					}
					channel->FlushCache();
					ClearChannelOutEvent(*channel);
				}
			}
		}
	}
}

bool YSFIOKernel::Init()
{
	if (0 > m_epollFd)
	{
		/* 未初始化过epoll句柄 */
		auto fd = epoll_create(1);
		if (0 > fd)
		{
			/* epoll句柄创建失败 */
			LOG("epoll_create error");
			return false;
		}
		m_epollFd = fd;
	}
	m_lChannel.clear();
	return true;
}

void YSFIOKernel::Fini()
{
	m_lChannel.clear();
	if (0 <= m_epollFd)
	{
		close(m_epollFd);
	}
	m_epollFd = -1;
}

bool YSFIOKernel::AddChannel(std::shared_ptr<IYSFIOChannel>& _channel)
{
	bool bRet = false;
	do
	{
		if (0 > m_epollFd)
		{
			LOG("需调用init初始化");
			break;
		}
		if (nullptr == _channel)
		{
			break;
		}
		epoll_event evn;
		evn.events = EPOLLIN;
		evn.data.ptr = static_cast<void*>(_channel.get());
		if (0 > epoll_ctl(m_epollFd, EPOLL_CTL_ADD, _channel->GetFd(), &evn))
		{
			LOG("epoll_ctl add error");
			break;
		}
		m_lChannel.push_back(_channel);
		bRet = true;
	} while (false);
	return bRet;
}

void YSFIOKernel::DelChannel(std::shared_ptr<IYSFIOChannel>& _channel)
{
	if (0 > m_epollFd)
	{
		LOG("需调用init初始化");
		return;
	}
	if (nullptr == _channel)
	{
		return;
	}
	if (0 > epoll_ctl(m_epollFd, EPOLL_CTL_DEL, _channel->GetFd(), nullptr))
	{
		LOG("epoll_ctl del error");
	}
	m_lChannel.remove(_channel);
	_channel->Fini();
	return;
}
