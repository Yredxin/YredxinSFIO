/*************************************************************************************
 *
 * 文 件 名:	IYSFIOHandle.h
 * 描    述:
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/9/30 15:41
 *
*************************************************************************************/
#ifndef _YSFIO_IHANDLE_
#define _YSFIO_IHANDLE_
#include <memory>
#include "AYSFIOHandle.h"
#include "IYSFIOChannel.h"

namespace YSFIO
{
	class IYSFIOHandle :
		public AYSFIOHandle
	{
	public:
		IYSFIOHandle() = default;
		virtual ~IYSFIOHandle() = default;

		/* 用户任务处理接口，如果有后续需要返回一个消息堆对象，对象交由内核管理 */
		virtual BytesMsg* ProcService(BytesMsg& _msg) = 0;
		virtual AYSFIOHandle* GetNextHandler(BytesMsg& _msg) = 0;

	private:
		// 通过 AYSFIOHandle 继承
		virtual std::unique_ptr<YSFIOMsg> InternelHandle(YSFIOMsg& _msg) override;
		virtual std::shared_ptr<AYSFIOHandle> GetNext(YSFIOMsg& _msg) override;
	};
}

#endif // !_YSFIO_IHANDLE_

