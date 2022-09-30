/*************************************************************************************
 *
 * 文 件 名:	IYSFIORole.cpp
 * 描    述:
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/9/30 10:51
 *
*************************************************************************************/
#include "IYSFIORole.h"
#include "util.h"
#include "IYSFIOProtocol.h"

using namespace std;
using namespace YSFIO;

void YSFIO::IYSFIORole::SetNextProcessor(IYSFIORole* _nextProc)
{
	pNextProc.reset(_nextProc);
}
void YSFIO::IYSFIORole::ClearNextProcessor()
{
	pNextProc.reset();
}
std::unique_ptr<YSFIOMsg> YSFIO::IYSFIORole::InternelHandle(YSFIOMsg& _msg)
{
	GET_REF2DATA(BytesMsg, oBytes, _msg);
	unique_ptr<YSFIOMsg> mRet = nullptr;
	if (SysIoReadyMsg::IN == oBytes.type)
	{
		GET_REF2DATA(UserDataMsg, uMsg, _msg);
		auto m = *(uMsg.pUserData.get());
		auto userData = ProcMsg(*(uMsg.pUserData));
		if (nullptr != userData)
		{
			unique_ptr<UserDataMsg> uData{ new UserDataMsg{oBytes} };
			uData->pUserData.reset(userData);
			mRet = std::move(uData);
		}
	}
	return mRet;
}

std::shared_ptr<AYSFIOHandle> YSFIO::IYSFIORole::GetNext(YSFIOMsg& _msg)
{
	GET_REF2DATA(UserDataMsg, uMsg, _msg);
	return pNextProc;
}
