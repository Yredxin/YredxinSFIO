/*************************************************************************************
 *
 * 文 件 名:	YSFIOKernel.h
 * 描    述:	框架内核，一切的起源
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/9/29 9:08
 *
*************************************************************************************/
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
		/* 将channel的输出事件添加到epoll */
		static bool	SetChannelOutEvent(IYSFIOChannel& _channel);
		/* 将channel的输出事件从epoll中移除 */
		static bool	ClearChannelOutEvent(IYSFIOChannel& _channel);
		friend class IYSFIOChannel;
	public:
		/* 外部内核初始化接口，转发到内部Init，用于创建实例对象+epoll句柄 */
		static bool YSFIO_Init();

		/* 外部接口，框架启动 */
		static void YSFIO_Run();

		/* 添加通道对象 */
		static bool YSFIO_AddChannel(std::shared_ptr<IYSFIOChannel> _channel);

		/* 移除通道对象 */
		static void YSFIO_DelChannel(std::shared_ptr<IYSFIOChannel> _channel);

		/* 外部内核销毁接口，转发到内部Fini，用于销毁实例对象+epoll句柄 */
		static void YSFIO_Fini();

		/* 向外输出 */
		static void YSFIO_SendOut(std::string& _output, std::shared_ptr<IYSFIOChannel> _channel);

		~YSFIOKernel();

	private:
		/* 隐藏构造和析构，实现单例模式 */
		YSFIOKernel();

		/* 单例工厂对象 */
		static std::unique_ptr<YSFIOKernel> pYSFIOKernel;

		/* 存储读写对象 */
		std::list<std::shared_ptr<IYSFIOChannel>> m_lChannel;

		/* epoll句柄 */
		int m_epollFd;

		/* 业务处理函数 */
		void Run();
		bool Init();
		void Fini();
		bool AddChannel(std::shared_ptr<IYSFIOChannel>& _channel);
		void DelChannel(std::shared_ptr<IYSFIOChannel>& _channel);
	};
}

#endif // !_YSFIO_KERNEL_
