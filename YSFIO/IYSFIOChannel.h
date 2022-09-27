#ifndef _YSFIO_ICHANNEL_
#define _YSFIO_ICHANNEL_
#include <string>

namespace YSFIO
{
	class IYSFIOChannel
	{
	public:
		/* 读通道 */
		virtual bool ReadFd(std::string& _input) = 0;
		/* 写通道 */
		virtual bool WriteFd(std::string& _output) = 0;
		/* 获取通道信息 */
		virtual std::string GetChannelInfo() = 0;
		/* 获取通道文件描述符 */
		virtual int GetFd() = 0;
	};
}

#endif // !_YSFIO_ICHANNEL_

