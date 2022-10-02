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
#include <string>
#include "AYSFIOHandle.h"

namespace YSFIO
{
	class IYSFIOChannel;
	class IYSFIORole;
	class BytesMsg;
	class UserData;
	class UserMsg;

	class IYSFIOProtocol :
		public AYSFIOHandle
	{
	public:
		IYSFIOProtocol() = default;
		virtual ~IYSFIOProtocol() = default;

		/* 流数据 -> 用户数据 */
		virtual UserData* Raw2Request(std::string& _input) = 0;

		/* 用户数据 -> 流数据 */
		virtual std::string	Response2Raw(UserData& _userData) = 0;

	protected:
		/* 获取上层业务处理角色 */
		virtual	IYSFIORole* GetProcMsg(UserMsg& _msg) = 0;

		/* 获取下层通道对象 */
		virtual IYSFIOChannel* GetSender(BytesMsg& _msg) = 0;
	private:
		// 通过 AYSFIOHandle 继承
		virtual YSFIOStreamMsg* InternelHandle(YSFIOStreamMsg& _msg) override;
		virtual AYSFIOHandle* GetNext(YSFIOStreamMsg& _msg) override;
	};
}

#endif // !_YSFIO_IPROTOCOL_

