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
#include "AYSFIOHandle.h"

void YSFIO::AYSFIOHandle::Handle(YSFIOMsg& _msg)
{
	auto msg = InternelHandle(_msg);
	if (nullptr == msg)
	{
		return;
	}
	auto next = GetNext(*msg);
	if (nullptr == next)
	{
		return;
	}
	next->Handle(*msg);
}

YSFIO::AYSFIOHandle::~AYSFIOHandle()
{
}
