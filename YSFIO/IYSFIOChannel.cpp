#include "IYSFIOChannel.h"
#include <unistd.h>
#include "YSFIOKernel.h"
using namespace YSFIO;

void IYSFIOChannel::FlushCache()
{
	while (!m_lCacheMsg.empty())
	{
		if (WriteFd(m_lCacheMsg.front()))
		{
			m_lCacheMsg.pop_front();
		}
		else
		{
			break;
		}
	}
}

void IYSFIOChannel::CleanCache()
{
	m_lCacheMsg.clear();
}

YSFIO::IYSFIOChannel::~IYSFIOChannel()
{
}

std::shared_ptr<AYSFIOMsg> YSFIO::IYSFIOChannel::InternelHandle(std::shared_ptr<AYSFIOMsg> _msg)
{
	GET_REF2DATA(ByteMsg, oByte, *_msg);
	std::shared_ptr<AYSFIOMsg> msgRet{ nullptr };

	switch (oByte.type)
	{
	case AYSFIOMsg::MsgType::MSG_IN:
	{
		auto msg = new ByteMsg{ _msg->type };
		if (ReadFd(*msg))
		{
			msg->msgInfo = GetChannelInfo();
			msgRet.reset(msg);
		}
		else
		{
			delete msg;
		}
		break;
	}
	case AYSFIOMsg::MsgType::MSG_OUT:
	{
		m_lCacheMsg.push_back(oByte);
		YSFIOKernel::GetInstance().ModIChannel(shared_from_this());
		break;
	}
	}
	return msgRet;
}

std::shared_ptr<AYSFIOHandle> YSFIO::IYSFIOChannel::GetNext(std::shared_ptr<AYSFIOMsg> _msg)
{
	std::shared_ptr<AYSFIOHandle> handle{ nullptr };
	if (AYSFIOMsg::MsgType::MSG_IN == _msg->type)
	{
		GET_REF2DATA(ByteMsg, oBytes, *_msg);
		handle = GetInputNextStage(std::make_shared<ByteMsg>(oBytes));
	}
	return handle;
}
