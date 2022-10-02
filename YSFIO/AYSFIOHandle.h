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

namespace YSFIO
{
	class YSFIOStreamMsg;

	class AYSFIOHandle
	{
	public:
		AYSFIOHandle() = default;
		virtual ~AYSFIOHandle() = 0;

		/* 责任链入口 */
		void Handle(YSFIOStreamMsg& _msg);

	protected:
		virtual YSFIOStreamMsg* InternelHandle(YSFIOStreamMsg& _msg) = 0;
		virtual AYSFIOHandle* GetNext(YSFIOStreamMsg& _msg) = 0;
	};
}

#endif // !_YSFIO_AHANDLE_

