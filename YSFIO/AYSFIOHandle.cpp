#include "AYSFIOHandle.h"
/*************************************************************************************
 *
 * 文 件 名:	AYSFIOHandle.cpp
 * 描    述:	责任链入口实现
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/9/29 9:08
 *
*************************************************************************************/
#include "YSFIOStreamMsg.h"

namespace YSFIO
{
	AYSFIOHandle::~AYSFIOHandle()
	{
	}

	void AYSFIOHandle::Handle(YSFIOStreamMsg& _msg)
	{
		auto newMsg = InternelHandle(_msg);
		if (nullptr != newMsg)
		{
			auto nextHandler = GetNext(*newMsg);
			if (nullptr != nextHandler)
			{
				nextHandler->Handle(*newMsg);
			}
			delete newMsg;
		}
	}
}