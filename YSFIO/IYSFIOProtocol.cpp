#include "IYSFIOProtocol.h"
/*************************************************************************************
 *
 * 文 件 名:	IYSFIOProtocol.cpp
 * 描    述:	协议层实现
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/9/30 9:35
 *
*************************************************************************************/
#include "YSFIOStreamMsg.h"
#include "IYSFIOChannel.h"
#include "IYSFIORole.h"
#include "util.h"

namespace YSFIO
{
	YSFIOStreamMsg* IYSFIOProtocol::InternelHandle(YSFIOStreamMsg& _msg)
	{
		GET_REF2DATA(YSFIOFrameMsg, oFrame, _msg);
		YSFIOStreamMsg* retMsg = nullptr;
		if (YSFIOFrameMsg::FrameMsgType::IN == oFrame.type)
		{
			GET_REF2DATA(BytesMsg, oBytes, oFrame);
			UserMsg* newMsg = new UserMsg(oFrame);
			newMsg->userData = Raw2Request(oBytes.bytesData);
			if (nullptr != newMsg->userData)
			{
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
			GET_REF2DATA(UserMsg, oUser, oFrame);
			BytesMsg* newMsg = new BytesMsg(oFrame);
			auto resMsg = Response2Raw(*(oUser.userData));
			newMsg->bytesData = *resMsg;
			delete resMsg;
			if (!newMsg->bytesData.empty())
			{
				retMsg = newMsg;
			}
			else
			{
				delete newMsg;
			}
			newMsg = nullptr;
		}
		return retMsg;
	}

	AYSFIOHandle* IYSFIOProtocol::GetNext(YSFIOStreamMsg& _msg)
	{
		GET_REF2DATA(YSFIOFrameMsg, oFrame, _msg);
		AYSFIOHandle* nextHandler = nullptr;
		if (YSFIOFrameMsg::FrameMsgType::IN == oFrame.type)
		{
			GET_REF2DATA(UserMsg, oUser, oFrame);
			nextHandler = GetProcMsg(oUser);
		}
		else if (YSFIOFrameMsg::FrameMsgType::OUT == oFrame.type)
		{
			GET_REF2DATA(BytesMsg, oBytes, oFrame);
			nextHandler = GetSender(oBytes);
		}
		return nextHandler;
	}
}