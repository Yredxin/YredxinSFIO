#ifndef _YSFIO_KERNEL_
#define _YSFIO_KERNEL_
#include <memory>

#include "../test/TestIn.h"

namespace YSFIO
{
	class YSFIOKernel
	{
	public:
		/* 框架启动 */
		void Run();

		/* 获取单例对象 */
		static YSFIOKernel& GetInstance();
	private:
		/* 隐藏构造和析构，实现单例模式 */
		YSFIOKernel();
		~YSFIOKernel();

		/* 单例工厂对象 */
		static YSFIOKernel m_instance;
		/* 存储读写对象 */
		std::shared_ptr<TestIn> in;
	};
}

#endif // !_YSFIO_KERNEL_
