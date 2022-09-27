#include "IYSFIOChannel.h"
#include <unistd.h>
#include "YSFIOKernel.h"
using namespace YSFIO;

void IYSFIOChannel::FlushCache()
{
	while (m_lCacheMsg.size() > 0)
	{
		auto& msg = m_lCacheMsg.front();
		if (!msg.empty())
		{
			write(GetFd(), msg.c_str(), msg.size());
		}
		m_lCacheMsg.pop_front();
	}
}

void IYSFIOChannel::CleanCache()
{
	m_lCacheMsg.clear();
}

bool IYSFIOChannel::SendMsg(std::string& _output)
{
	m_lCacheMsg.push_back(_output);
	return YSFIOKernel::GetInstance().ModIChannel(shared_from_this());
}
