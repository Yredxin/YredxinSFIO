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
#include <memory>
#include "AYSFIOHandle.h"

namespace YSFIO
{
	struct SysIoReadyMsg : public YSFIOMsg
	{
		enum MsgType :char
		{
			IN = 1,			/* 输入事件 */
			OUT = 2,		/* 输出事件 */
			INOUT = 3		/* 输入+输出事件 */
		}type;
		SysIoReadyMsg(const MsgType& _type) :type{ _type } {};
	};

	/* 消息对象 */
	struct BytesMsg :public SysIoReadyMsg
	{
		BytesMsg(const SysIoReadyMsg& _sysMsg) :SysIoReadyMsg{ _sysMsg.type } {};
		std::string msgData;
		std::string msgInfo;
	};

	class IYSFIOChannel :
		public AYSFIOHandle
	{
		std::list<std::string> m_lCacheMsg;
		friend class YSFIOKernel;
	public:
		IYSFIOChannel() = default;
		virtual ~IYSFIOChannel() = default;
		/* 使用者初始化，内核会自动调用 */
		virtual bool Init() = 0;
		virtual void Fini() = 0;

		/* 读通道 */
		virtual bool ReadFd(std::string& _input) = 0;
		/* 写通道 */
		virtual bool WriteFd(std::string& _output) = 0;
		/* 获取通道信息 */
		virtual std::string GetChannelInfo() = 0;
		/* 获取通道文件描述符 */
		virtual int GetFd() = 0;
		/* 是否有缓存消息 */
		inline bool IsCache() { return !m_lCacheMsg.empty(); };
		/* 获取下一个消息类型，在堆上创建，内存交给框架 */
		virtual IYSFIOChannel* GetInputNextStage(BytesMsg& _msg) = 0;
	private:
		/* 刷新缓存 */
		void FlushCache();
		/* 读写事件处理 */
		virtual std::unique_ptr<YSFIOMsg> InternelHandle(YSFIOMsg& _msg) override final;
		/* 获取数据处理下一个事件 */
		virtual std::unique_ptr<AYSFIOHandle> GetNext(YSFIOMsg& _msg) override final;
	};
}

#endif // !_YSFIO_ICHANNEL_

