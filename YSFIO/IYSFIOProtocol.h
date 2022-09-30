/*************************************************************************************
 *
 * 文 件 名:	IYSFIOProtocol.h
 * 描    述:	责任链中的协议层，用于流式数据与用户数据之间的转换
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/9/30 9:35
 *
*************************************************************************************/
#ifndef _YSFIO_IPROTOCOL_
#define _YSFIO_IPROTOCOL_
#include <memory>

#include "IYSFIOChannel.h"

namespace YSFIO
{
	struct UserData
	{
		UserData() = default;
		virtual ~UserData() = default;
	};
	struct UserDataMsg :public BytesMsg
	{
		UserDataMsg(const BytesMsg& _bytesMsg) :BytesMsg{ _bytesMsg } {};
		std::shared_ptr<UserData> pUserData = nullptr;
		virtual ~UserDataMsg() = default;
	};
	class IYSFIOProtocol
	{
	public:
		IYSFIOProtocol() = default;
		virtual ~IYSFIOProtocol() = default;

	};
}

#endif // !_YSFIO_IPROTOCOL_

