/*************************************************************************************
 *
 * 文 件 名:	IYSFIOChannel.cpp
 * 描    述:
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/9/29 9:08
 *
*************************************************************************************/
#include "IYSFIOChannel.h"
#include <unistd.h>
#include "YSFIOKernel.h"
using namespace YSFIO;

void IYSFIOChannel::FlushCache()
{
	while (IsCache())
	{
		if (!WriteFd(m_lCacheMsg.front()))
		{
			break;
		}
		m_lCacheMsg.pop_front();
	}
	m_lCacheMsg.clear();
}

std::unique_ptr<YSFIOMsg> YSFIO::IYSFIOChannel::InternelHandle(YSFIOMsg& _msg)
{
	GET_REF2DATA(SysIoReadyMsg, oSysMsg, _msg);
	std::unique_ptr<YSFIOMsg> pRetMsg = nullptr;
	if (SysIoReadyMsg::OUT & oSysMsg.type)
	{
		/* 事件包含写事件 */
		GET_REF2DATA(BytesMsg, oBytes, oSysMsg);
		if (!IsCache())
		{
			/* 数据缓存中没有数据，需要将channel的epoll写事件打开 */
			YSFIOKernel::SetChannelOutEvent(*this);
		}
		/* 数据写入缓存 */
		m_lCacheMsg.push_back(oBytes.msgData);
		/* 移除写事件，以为是通道类，所以不能将写事件继续传递 */
		oSysMsg.type = static_cast<SysIoReadyMsg::MsgType>(oSysMsg.type & ~SysIoReadyMsg::OUT);
	}
	if (SysIoReadyMsg::IN & oSysMsg.type)
	{
		/* 事件包含读事件 */
		auto msg = new BytesMsg{ oSysMsg };
		if (true == ReadFd(msg->msgData))
		{
			msg->msgInfo = GetChannelInfo();
			pRetMsg.reset(msg);
		}
		else
		{
			delete msg;
		}
		msg = nullptr;
	}
	return std::move(pRetMsg);
}

std::unique_ptr<AYSFIOHandle> YSFIO::IYSFIOChannel::GetNext(YSFIOMsg& _msg)
{
	GET_REF2DATA(SysIoReadyMsg, oSysMsg, _msg);
	std::unique_ptr<AYSFIOHandle> pRetHandle = nullptr;
	if (SysIoReadyMsg::IN & oSysMsg.type)
	{
		GET_REF2DATA(BytesMsg, oBytes, _msg);
		pRetHandle.reset(GetInputNextStage(oBytes));
	}
	return std::move(pRetHandle);
}
