#ifndef _YSFIO_KERNEL_
#define _YSFIO_KERNEL_
#include <memory>
#include <list>
#include "util.h"
#include "IYSFIOChannel.h"

namespace YSFIO
{
	class YSFIOKernel
	{
	public:

		/* 初始化epoll句柄 */
		bool Init();

		/* 框架启动 */
		void Run();

		/* 获取单例对象 */
		static YSFIOKernel& GetInstance();

		/* 添加通道对象 */
		bool AddIChannel(std::shared_ptr<IYSFIOChannel> _channel);

		/* 移除通道对象 */
		bool DelIChannel(std::shared_ptr<IYSFIOChannel> _channel);

		/* 给通道添加写事件检测 */
		bool ModIChannel(std::shared_ptr<IYSFIOChannel> _channel);

		/* 移除通道写事件检测 */
		bool RemIChannel(std::shared_ptr<IYSFIOChannel> _channel);

		/* 销毁epoll句柄 */
		void Fini();

		/* 向外输出 */
		void SendOut(std::string& _output, std::shared_ptr<AYSFIOHandle> _handle);

	private:
		/* 隐藏构造和析构，实现单例模式 */
		YSFIOKernel();
		~YSFIOKernel();

		/* 单例工厂对象 */
		static YSFIOKernel m_instance;
		/* 存储读写对象 */
		// std::shared_ptr<IYSFIOChannel> m_channel;
		std::list<std::shared_ptr<IYSFIOChannel>> m_lChannel;

		/* epoll句柄 */
		int m_epollFd;

		/* 是否初始化 */
		bool IsInit();
	};
}

#endif // !_YSFIO_KERNEL_
