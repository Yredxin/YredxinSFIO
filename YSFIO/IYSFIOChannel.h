#ifndef _YSFIO_ICHANNEL_
#define _YSFIO_ICHANNEL_
#include <string>
#include <list>
#include <memory>
#include "AYSFIOHandle.h"

namespace YSFIO
{
	/* 消息对象 */
	struct ByteMsg :public AYSFIOMsg
	{
		ByteMsg(AYSFIOMsg::MsgType _type) :AYSFIOMsg{ _type } {};
		std::string msgData;
		std::string msgInfo;
	};

	class IYSFIOChannel :
		public std::enable_shared_from_this<IYSFIOChannel>,
		public AYSFIOHandle
	{
		std::list<ByteMsg> m_lCacheMsg;
	public:
		/* 读通道 */
		virtual bool ReadFd(ByteMsg& _input) = 0;
		/* 写通道 */
		virtual bool WriteFd(ByteMsg& _output) = 0;
		/* 获取通道信息 */
		virtual std::string GetChannelInfo() = 0;
		/* 获取通道文件描述符 */
		virtual int GetFd() = 0;
		/* 是否有缓存消息 */
		inline bool IsCache() { return !m_lCacheMsg.empty(); };
		/* 刷新缓存 */
		void FlushCache();
		/* 清理缓存 */
		void CleanCache();
		/* 发送 */
		virtual ~IYSFIOChannel() = 0;
		/* 获取下一个消息类型 */
		virtual std::shared_ptr<IYSFIOChannel> GetInputNextStage(std::shared_ptr<ByteMsg> _msg) = 0;
	private:
		// 通过 AYSFIOHandle 继承
		virtual std::shared_ptr<AYSFIOMsg> InternelHandle(std::shared_ptr<AYSFIOMsg> _msg) final;
		virtual std::shared_ptr<AYSFIOHandle> GetNext(std::shared_ptr<AYSFIOMsg> _msg) final;
	};
}

#endif // !_YSFIO_ICHANNEL_

