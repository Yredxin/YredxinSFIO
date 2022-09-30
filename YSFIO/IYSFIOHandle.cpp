/*************************************************************************************
 *
 * 文 件 名:	IYSFIOHandle.cpp
 * 描    述:
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/9/30 15:41
 *
*************************************************************************************/
#include "IYSFIOHandle.h"
#include "util.h"

std::unique_ptr<YSFIO::YSFIOMsg> YSFIO::IYSFIOHandle::InternelHandle(YSFIO::YSFIOMsg& _msg)
{
	GET_REF2DATA(BytesMsg, oBytes, _msg);
	std::unique_ptr<YSFIO::YSFIOMsg> pRet{ ProcService(oBytes) };
	return pRet;
}

std::shared_ptr<YSFIO::AYSFIOHandle> YSFIO::IYSFIOHandle::GetNext(YSFIO::YSFIOMsg& _msg)
{
	GET_REF2DATA(BytesMsg, oBytes, _msg);
	std::shared_ptr<YSFIO::AYSFIOHandle> pRet{ GetNextHandler(oBytes) };
	return pRet;
}
