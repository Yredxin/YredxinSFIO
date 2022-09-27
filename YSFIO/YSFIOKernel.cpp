#include "YSFIOKernel.h"
#include <iostream>
#include <string>
#include <sys/epoll.h>
#include <unistd.h>
#include <algorithm>
#include "util.h"
using namespace std;
using namespace YSFIO;

YSFIOKernel YSFIOKernel::m_instance{};

bool YSFIO::YSFIOKernel::Init()
{
	bool bRet = false;

	do
	{
		int fd = epoll_create(1);
		if (0 > fd)
		{
			LOG("epoll_create error");
			break;
		}
		m_epollFd = fd;
		bRet = true;
	} while (false);
	return bRet;
}

void YSFIOKernel::Run()
{
	if (!IsInit())
	{
		/* 未初始化 */
		return;
	}
	while (true)
	{
		epoll_event evs[128] = { 0 };
		int evNum = epoll_wait(m_epollFd, evs, sizeof(evs) / sizeof(epoll_event), -1);
		if (0 > evNum)
		{
			/* 信号中断 */
			if (EINTR == errno)
			{
				continue;
			}
			/* 出现意外 */
			LOG("epoll_wait error");
			break;
		}
		if (0 == evNum)
		{
			/* todo: 如果设置超时则处理 */
			continue;
		}
		/* 有事件产生 */
		for (size_t i = 0; i < evNum; i++)
		{
			string sBuf;
			auto channel = static_cast<IYSFIOChannel*>(evs[i].data.ptr);
			if (nullptr == channel)
			{
				continue;
			}
			if (EPOLLIN & evs[i].events)
			{
				channel->Handle(std::make_shared<ByteMsg>(AYSFIOMsg::MsgType::MSG_IN));
			}
			if (EPOLLOUT & evs[i].events)
			{
				for (auto& iter : m_lChannel)
				{
					if (iter.get() == channel)
					{
						RemIChannel(iter);
						break;
					}
				}
				/* 写事件产生 */
				if (channel->IsCache())
				{
					channel->FlushCache();
					channel->CleanCache();
				}
			}
		}
	}
}

YSFIOKernel& YSFIOKernel::GetInstance()
{
	return m_instance;
}

bool YSFIO::YSFIOKernel::AddIChannel(std::shared_ptr<IYSFIOChannel> _channel)
{
	bool bRet = false;
	do
	{
		if (!IsInit())
		{
			/* 未初始化 */
			break;
		}
		epoll_event ev;
		ev.events = EPOLLIN;
		ev.data.ptr = _channel.get();
		if (0 > epoll_ctl(m_epollFd, EPOLL_CTL_ADD, _channel->GetFd(), &ev))
		{
			LOG("epoll_ctl add error");
			break;
		}
		m_lChannel.push_back(_channel);
		bRet = true;
	} while (false);
	return bRet;
}

bool YSFIO::YSFIOKernel::DelIChannel(std::shared_ptr<IYSFIOChannel> _channel)
{
	bool bRet = false;
	do
	{
		if (!IsInit())
		{
			/* 未初始化 */
			break;
		}
		auto iter = std::find(m_lChannel.begin(), m_lChannel.end(), _channel);
		if (m_lChannel.end() == iter)
		{
			/* 参数错误 */
			LOG("param error");
			break;
		}
		if (0 > epoll_ctl(m_epollFd, EPOLL_CTL_DEL, _channel->GetFd(), NULL))
		{
			LOG("epoll_ctl del error");
			break;
		}
		m_lChannel.erase(iter);
		bRet = true;
	} while (false);
	return bRet;
}

bool YSFIO::YSFIOKernel::ModIChannel(std::shared_ptr<IYSFIOChannel> _channel)
{
	bool bRet = false;
	do
	{
		if (!IsInit())
		{
			/* 未初始化 */
			break;
		}
		if (m_lChannel.end() == std::find(m_lChannel.begin(), m_lChannel.end(), _channel))
		{
			/* 参数错误 */
			LOG("param error");
			break;
		}
		epoll_event ev;
		ev.events = EPOLLIN | EPOLLOUT;
		ev.data.ptr = _channel.get();
		if (0 > epoll_ctl(m_epollFd, EPOLL_CTL_MOD, _channel->GetFd(), &ev))
		{
			LOG("epoll_ctl mod error");
			break;
		}
		bRet = true;
	} while (false);
	return bRet;
}

bool YSFIO::YSFIOKernel::RemIChannel(std::shared_ptr<IYSFIOChannel> _channel)
{
	bool bRet = false;
	do
	{
		if (!IsInit())
		{
			/* 未初始化 */
			break;
		}
		if (m_lChannel.end() == std::find(m_lChannel.begin(), m_lChannel.end(), _channel))
		{
			/* 参数错误 */
			LOG("param error");
			break;
		}
		epoll_event ev;
		ev.events = EPOLLIN;
		ev.data.ptr = _channel.get();
		if (0 > epoll_ctl(m_epollFd, EPOLL_CTL_MOD, _channel->GetFd(), &ev))
		{
			LOG("epoll_ctl mod error");
			break;
		}
		bRet = true;
	} while (false);
	return bRet;
}

void YSFIO::YSFIOKernel::Fini()
{
	if (0 <= m_epollFd)
	{
		close(m_epollFd);
	}
}

void YSFIO::YSFIOKernel::SendOut(std::string& _output, std::shared_ptr<AYSFIOHandle> _handle)
{
	auto msg = std::make_shared<ByteMsg>(AYSFIOMsg::MsgType::MSG_OUT);
	msg->msgData = _output;
	_handle->Handle(msg);
}

YSFIOKernel::YSFIOKernel() :
	m_lChannel{ nullptr },
	m_epollFd{ -1 }
{
}

YSFIOKernel::~YSFIOKernel()
{
	Fini();
}

bool YSFIO::YSFIOKernel::IsInit()
{
	if (0 > m_epollFd)
	{
		LOG("需要调用init进行初始化");
		return false;
	}
	return true;
}
