/*************************************************************************************
 *
 * 文 件 名:	AYSFIOHandle.h
 * 描    述:	责任链基类
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/9/29 9:08
 *
*************************************************************************************/
#ifndef _YSFIO_AHANDLE_
#define _YSFIO_AHANDLE_
#include <memory>

namespace YSFIO
{
	struct YSFIOMsg
	{
		YSFIOMsg() = default;
		virtual ~YSFIOMsg() = default;
	};

	class AYSFIOHandle
	{
	public:
		void Handle(YSFIOMsg& _msg);
		virtual std::unique_ptr<YSFIOMsg> InternelHandle(YSFIOMsg& _msg) = 0;
		virtual std::unique_ptr<AYSFIOHandle> GetNext(YSFIOMsg& _msg) = 0;
		virtual ~AYSFIOHandle();
	};
}

#endif // !_YSFIO_AHANDLE_

