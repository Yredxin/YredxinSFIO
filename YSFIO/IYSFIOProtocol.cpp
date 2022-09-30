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
#include "IYSFIOProtocol.h"
#include "util.h"
#include "IYSFIORole.h"

using namespace std;
using namespace YSFIO;

std::unique_ptr<YSFIOMsg> YSFIO::IYSFIOProtocol::InternelHandle(YSFIOMsg& _msg)
{
	unique_ptr<YSFIOMsg> pRetMsg = nullptr;
	GET_REF2DATA(BytesMsg, oBytes, _msg);
	if (SysIoReadyMsg::IN == oBytes.type)
	{
		/* 协议转换，将流数据转为用户数据 */
		unique_ptr<UserData> pUserData = nullptr;
		pUserData.reset(Raw2Request(oBytes.msgData));
		if (nullptr != pUserData)
		{
			auto userDataMsg = new UserDataMsg{ oBytes };
			userDataMsg->pUserData = move(pUserData);
			pRetMsg.reset(userDataMsg);
			userDataMsg = nullptr;
		}
	}
	else
	{
		/* 将用户数据转换为流数据 */
		GET_REF2DATA(UserDataMsg, oUserData, _msg);
		unique_ptr<string> pData = nullptr;
		pData.reset(Response2Raw(*(oUserData.pUserData)));
		if (nullptr != pData)
		{
			auto bytesMsg = new BytesMsg{ oBytes };
			bytesMsg->msgData = *pData;
			pRetMsg.reset(bytesMsg);
			bytesMsg = nullptr;
		}
	}
	return pRetMsg;
}

std::shared_ptr<AYSFIOHandle> YSFIO::IYSFIOProtocol::GetNext(YSFIOMsg& _msg)
{
	shared_ptr<AYSFIOHandle> pRetHandler = nullptr;
	GET_REF2DATA(BytesMsg, oBytes, _msg);
	if (SysIoReadyMsg::IN == oBytes.type)
	{
		GET_REF2DATA(UserDataMsg, oUserData, _msg);
		// 转到上层
		pRetHandler.reset(GetProcRole(oUserData));
	}
	else
	{
		pRetHandler.reset(GetSenderChannel(oBytes));
	}
	return pRetHandler;
}
