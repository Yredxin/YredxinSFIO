/*************************************************************************************
 *
 * 文 件 名:	YSFIOStreamMsg.cpp
 * 描    述:
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/10/1 17:14
 *
*************************************************************************************/
#include "YSFIOStreamMsg.h"


namespace YSFIO
{
	YSFIOFrameMsg::YSFIOFrameMsg(const FrameMsgType& _type) :
		type{ _type }
	{
	}

	BytesMsg::BytesMsg(const YSFIOFrameMsg& _base) :
		YSFIOFrameMsg{ _base.type }
	{
	}

	UserMsg::UserMsg(const BytesMsg& _base) :
		BytesMsg{ _base },
		userData{ nullptr }
	{
	}

	UserMsg::~UserMsg()
	{
		if (nullptr != userData)
		{
			delete userData;
		}
	}

}
