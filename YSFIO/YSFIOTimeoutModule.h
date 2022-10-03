/*************************************************************************************
 *
 * 文 件 名:	YSFIOTimeoutModule.h
 * 描    述:	定时任务处理，开发者需继承IYSFIOTask实现自己的任务
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/10/2 20:30
 *
*************************************************************************************/
#ifndef _AYSFIO_TIMEOUT_MODULE_
#define _AYSFIO_TIMEOUT_MODULE_
#include <memory>
#include <vector>
#include <list>
#include "AYSFIOHandle.h"
#include "IYSFIOChannel.h"

namespace YSFIO
{
	constexpr int TimerQueryLen = 10;
	/* 定时器任务抽象类 */
	class IYSFIOTask
	{
		int m_pos;	   /* 记录所在位置，方便移除 */
		int	m_rounds;  /* 记录圈数 */
		bool m_isLive; /* 任务是否存活 */
		friend class YSFIOTimerTaskProc;
	public:
		IYSFIOTask();
		virtual ~IYSFIOTask() = default;
		virtual void Proc() = 0;
		virtual int TimerSec() = 0;
		void KillTask() { m_isLive = false; };
		inline bool IsLive() { return m_isLive; };
	};

	/* 定时器任务触发类 */
	class YSFIOTimerTaskProc :
		public AYSFIOHandle
	{
		static std::unique_ptr<YSFIOTimerTaskProc> proc;
		YSFIOTimerTaskProc();
	public:
		~YSFIOTimerTaskProc();
		static bool Init();
		static void Fini();
		static YSFIOTimerTaskProc& GetTaskProc();
		static bool AddTask(IYSFIOTask* _task);
		static void DelTask(IYSFIOTask* _task);
	private:
		std::vector<std::list<IYSFIOTask*>> m_lTask;
		int m_pos;
		// 通过 AYSFIOHandle 继承
		virtual YSFIOStreamMsg* InternelHandle(YSFIOStreamMsg& _msg) override;
		virtual AYSFIOHandle* GetNext(YSFIOStreamMsg& _msg) override;
	};

	/* 定时器管理类 */
	class YSFIOTimerMngChannel :
		public IYSFIOChannel
	{
		int m_timerFd;
	public:
		YSFIOTimerMngChannel();
		~YSFIOTimerMngChannel();
		// 通过 IYSFIOChannel 继承
		virtual bool Init() override;
		virtual void Fini() override;
		virtual bool ReadFd(std::string& _input) override;
		virtual bool WriteFd(std::string& _output) override;
		/* 超时触发 */
		virtual int GetFd() override;
		virtual std::string GetChannelInfo() override;
		virtual AYSFIOHandle* GetInputNextStage(BytesMsg& _input) override;
	};
}

#endif // !_AYSFIO_TIMEOUT_MODULE_

