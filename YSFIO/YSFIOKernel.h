#ifndef _YSFIO_KERNEL_
#define _YSFIO_KERNEL_
#include <memory>
#include "util.h"

#include "../test/TestIn.h"

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

		/* 添加读写对象 */
		bool AddTestIn(std::shared_ptr<TestIn> _in);

		/* 销毁epoll句柄 */
		void Fini();

	private:
		/* 隐藏构造和析构，实现单例模式 */
		YSFIOKernel();
		~YSFIOKernel();

		/* 单例工厂对象 */
		static YSFIOKernel m_instance;
		/* 存储读写对象 */
		std::shared_ptr<TestIn> in;

		/* epoll句柄 */
		int m_epollFd;

		/* 是否初始化 */
		bool IsInit();
	};
}

#endif // !_YSFIO_KERNEL_
