#ifndef _YSFIO_ICHANNEL_
#define _YSFIO_ICHANNEL_
#include <string>
#include <list>
#include <memory>

namespace YSFIO
{
	class IYSFIOChannel :
		public std::enable_shared_from_this<IYSFIOChannel>
	{
		std::list<std::string> m_lCacheMsg;
	public:
		/* 读通道 */
		virtual bool ReadFd(std::string& _input) = 0;
		/* 业务处理 */
		virtual void Business(std::string& _msg) = 0;
		/* 写通道 */
		virtual bool WriteFd(std::string& _output) = 0;
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
		bool SendMsg(std::string& _output);
	};
}

#endif // !_YSFIO_ICHANNEL_

