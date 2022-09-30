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
#include <string>
#include "AYSFIOHandle.h"
#include "IYSFIOChannel.h"

namespace YSFIO
{
	class IYSFIORole;

	struct UserData
	{
		UserData() = default;
		virtual ~UserData() = default;
	};
	struct UserDataMsg :public BytesMsg
	{
		UserDataMsg(const BytesMsg& _bytesMsg) :BytesMsg{ _bytesMsg } {};
		std::unique_ptr<UserData> pUserData = nullptr;
		virtual ~UserDataMsg() = default;
	};

	class IYSFIOProtocol :
		public AYSFIOHandle
	{
	public:
		IYSFIOProtocol() = default;
		virtual ~IYSFIOProtocol() = default;
		/* 流数据->用户数据 */
		virtual UserData* Raw2Request(std::string& _input) = 0;
		/* 用户数据->流数据 */
		virtual std::string* Response2Raw(UserData& _userData) = 0;

		/* 获取角色对象 */
		virtual	IYSFIORole* GetProcRole(UserDataMsg& _msg) = 0;
		/* 获取通道对象 */
		virtual	IYSFIOChannel* GetSenderChannel(BytesMsg& _msg) = 0;
	private:
		// 通过 AYSFIOHandle 继承
		virtual std::unique_ptr<YSFIOMsg> InternelHandle(YSFIOMsg& _msg) override;
		virtual std::shared_ptr<AYSFIOHandle> GetNext(YSFIOMsg& _msg) override;
	};
}

#endif // !_YSFIO_IPROTOCOL_

