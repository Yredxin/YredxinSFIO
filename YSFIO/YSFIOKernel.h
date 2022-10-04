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

struct epoll_event;

namespace YSFIO
{
	class AYSFIOHandle;
	class IYSFIOChannel;
	class IYSFIOProtocol;
	class IYSFIORole;
	class UserData;

	class YSFIOKernel
	{
		static void SetChannelOut(IYSFIOChannel* _channel);
		static void ClearChannelOut(IYSFIOChannel* _channel);
		friend class IYSFIOChannel;
	public:
		/* 开发者必须调用Init初始化 */
		static bool Init();
		/* 开始运行框架 */
		static void Run();
		/* 结束时需调用Fini释放资源，防止资源泄露 */
		static void Fini();
		/* 添加通道类 */
		static bool AddChannel(IYSFIOChannel* _channel);
		/* 移除通道类，移除后此类堆会被释放 */
		static void DelChannel(IYSFIOChannel* _channel);
		/* 添加协议类 */
		static bool AddProtocol(IYSFIOProtocol* _protocol);
		/* 移除协议类，移除后此类堆会被释放 */
		static void DelProtocol(IYSFIOProtocol* _protocol);
		/* 添加角色类 */
		static bool AddRole(IYSFIORole* _role);
		/* 移除角色类，移除后此类堆会被释放 */
		static void DelRole(IYSFIORole* _role);
		/* 用于指定输出通道 */
		static void SendOut(std::string& _output, IYSFIOChannel* _outChannel);
		static void SendOut(UserData& _userData, IYSFIOProtocol* _outProto);

		/* 全局单例为智能指针，释放时需要公开私有函数 */
		~YSFIOKernel();

		/* 框架退出 */
		static void Exit();

		/* 获取字符串描述的通道列表，参数2为输出参数 */
		static std::list<IYSFIOChannel*> GetInfoChannel(const std::string& _info);
		/* 获取第一个匹配的字符串描述的通道，参数2为输出参数 */
		static IYSFIOChannel* GetFirstInfoChannel(const std::string& _info);

		/* 获取所有的通道 */
		static std::list<IYSFIOChannel*>& GetAllChannel();
		/* 获取所有的协议 */
		static std::list<IYSFIOProtocol*>& GetAllProtocol();
		/* 获取所有的角色 */
		static std::list<IYSFIORole*>& GetAllRole();
	private:
		/* 禁止构造 */
		YSFIOKernel();

		bool m_Init();
		void m_Run();
		void m_Fini();
		bool m_AddChannel(IYSFIOChannel* _channel);
		void m_DelChannel(IYSFIOChannel* _channel);
		bool m_AddProtocol(IYSFIOProtocol* _protocol);
		void m_DelProtocol(IYSFIOProtocol* _protocol);
		bool m_AddRole(IYSFIORole* _role);
		void m_DelRole(IYSFIORole* _role);

		/* 事件处理函数 */
		void ProcEvns(epoll_event& _evn);

	private:
		bool m_isRun;
		/* 全局单例 */
		static std::unique_ptr<YSFIOKernel> kernel;
		/* epoll句柄 */
		int m_epollFd;

		/* 对象列表 */
		std::list<IYSFIOChannel*> m_lChannel;
		std::list<IYSFIOProtocol*> m_lProtocol;
		std::list<IYSFIORole*> m_lRole;
	};
}

#endif // !_YSFIO_KERNEL_
