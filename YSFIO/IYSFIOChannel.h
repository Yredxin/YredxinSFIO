/*************************************************************************************
 *
 * 文 件 名:	IYSFIOChannel.h
 * 描    述:	责任链中的通道层，用于输入输出
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/9/29 9:08
 *
*************************************************************************************/
#ifndef _YSFIO_ICHANNEL_
#define _YSFIO_ICHANNEL_
#include <string>
#include <list>
#include "AYSFIOHandle.h"

namespace YSFIO
{
	class BytesMsg;

	class IYSFIOChannel :
		public AYSFIOHandle
	{
	public:
		IYSFIOChannel() = default;
		virtual ~IYSFIOChannel() = default;

		/* 此接口用于初始化自定义资源，内核自动调用 */
		virtual bool Init() = 0;

		/* 此接口用于释放自定义资源 */
		virtual void Fini() = 0;

		/* 此接口用于自定义通道读写处理 */
		virtual bool ReadFd(std::string& _input) = 0;
		virtual bool WriteFd(std::string& _output) = 0;

		/* 此接口用于返回通道文件描述符 */
		virtual int GetFd() = 0;

		/* 此接口用于返回通道描述信息 */
		virtual std::string GetChannelInfo() = 0;

		/* 此接口用于获取下一个工作者 */
		virtual AYSFIOHandle* GetInputNextStage(BytesMsg& _input) = 0;

		/* 刷新缓存 */
		void FlushCache();

		/* 是否有缓存 */
		inline bool HasCache() { return !m_writeBuf.empty(); };

	private:
		/* 需要输出数据临时存放 */
		std::list<std::string> m_writeBuf;

		// 通过 AYSFIOHandle 继承
		virtual YSFIOStreamMsg* InternelHandle(YSFIOStreamMsg& _msg) override;
		virtual AYSFIOHandle* GetNext(YSFIOStreamMsg& _msg) override;
	};
}

#endif // !_YSFIO_ICHANNEL_

