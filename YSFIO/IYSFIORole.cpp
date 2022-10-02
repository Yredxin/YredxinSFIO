#include "IYSFIORole.h"
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
#include "YSFIOStreamMsg.h"
#include "YSFIOStreamMsg.h"
#include "util.h"

namespace YSFIO
{
	void IYSFIORole::SetNextProcessor(IYSFIORole* _role)
	{
		m_nextRole = _role;
	}
	void IYSFIORole::ClearNextProcessor()
	{
		m_nextRole = nullptr;
	}
	YSFIOStreamMsg* IYSFIORole::InternelHandle(YSFIOStreamMsg& _msg)
	{
		GET_REF2DATA(YSFIOFrameMsg, oFram, _msg);
		YSFIOStreamMsg* retMsg = nullptr;
		if (YSFIOFrameMsg::FrameMsgType::IN == oFram.type)
		{
			GET_REF2DATA(UserMsg, oUser, oFram);
			UserMsg* newMsg = new UserMsg{ oFram };
			newMsg->userData = ProcMsg(*(oUser.userData));
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
		return retMsg;
	}
	AYSFIOHandle* IYSFIORole::GetNext(YSFIOStreamMsg& _msg)
	{
		GET_REF2DATA(UserMsg, oUser, _msg);
		return m_nextRole;
	}
	IYSFIORole::IYSFIORole() :
		m_nextRole{ nullptr }
	{
	}
	IYSFIORole::~IYSFIORole()
	{
	}
}