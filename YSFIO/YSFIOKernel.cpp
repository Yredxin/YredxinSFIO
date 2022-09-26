#include "YSFIOKernel.h"
#include <iostream>
#include <string>
#include <sys/epoll.h>
#include <unistd.h>
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
			auto in = static_cast<TestIn*>(evs[i].data.ptr);
			if (nullptr == in)
			{
				continue;
			}
			string buf;
			in->ReadFd(buf);
			in->WriteFd(buf);
		}
	}
}

YSFIOKernel& YSFIOKernel::GetInstance()
{
	return m_instance;
}

bool YSFIO::YSFIOKernel::AddTestIn(std::shared_ptr<TestIn> _in)
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
		ev.data.ptr = _in.get();
		if (0 > epoll_ctl(m_epollFd, EPOLL_CTL_ADD, 0, &ev))
		{
			LOG("epoll_ctl add error");
			break;
		}
		in = _in;
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

YSFIOKernel::YSFIOKernel() :
	in{ nullptr },
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
