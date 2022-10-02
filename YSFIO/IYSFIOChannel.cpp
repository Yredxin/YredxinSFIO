#include "IYSFIOChannel.h"
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

#include "util.h"
#include "YSFIOStreamMsg.h"
#include "YSFIOKernel.h"

namespace YSFIO
{
	void IYSFIOChannel::FlushCache()
	{
		while (!m_writeBuf.empty())
		{
			if (!WriteFd(m_writeBuf.front()))
			{
				/* 没有实现写功能 */
				break;
			}
			m_writeBuf.pop_front();
		}
	}

	YSFIOStreamMsg* IYSFIOChannel::InternelHandle(YSFIOStreamMsg& _msg)
	{
		GET_REF2DATA(YSFIOFrameMsg, oFrame, _msg);
		YSFIOStreamMsg* retMsg = nullptr;
		if (YSFIOFrameMsg::FrameMsgType::IN == oFrame.type)
		{
			/* 消息类型为读 */
			BytesMsg* newMsg = new BytesMsg{ oFrame };
			if (ReadFd(newMsg->bytesData))
			{
				newMsg->channelInfo = GetChannelInfo();
				retMsg = newMsg;
			}
			else
			{
				delete newMsg;
			}
			newMsg = nullptr;
		}
		else if (YSFIOFrameMsg::FrameMsgType::OUT == oFrame.type)
		{
			GET_REF2DATA(BytesMsg, oBytes, oFrame);
			/* 消息类型为写 */
			if (m_writeBuf.empty())
			{
				YSFIOKernel::SetChannelOut(this);
			}
			m_writeBuf.push_back(oBytes.bytesData);
		}
		return retMsg;
	}

	AYSFIOHandle* IYSFIOChannel::GetNext(YSFIOStreamMsg& _msg)
	{
		GET_REF2DATA(YSFIOFrameMsg, oFrame, _msg);
		AYSFIOHandle* nextHandler = nullptr;
		if (YSFIOFrameMsg::FrameMsgType::IN == oFrame.type)
		{
			GET_REF2DATA(BytesMsg, oBytes, oFrame);
			nextHandler = GetInputNextStage(oBytes);
		}
		return nextHandler;
	}
}
