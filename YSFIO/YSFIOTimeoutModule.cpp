/*************************************************************************************
 *
 * 文 件 名:	YSFIOTimeoutModule.cpp
 * 描    述:	超时任务处理类实现
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/10/2 20:30
 *
*************************************************************************************/
#include <unistd.h>
#include <sys/timerfd.h>
#include <stdint.h>
#include "YSFIOTimeoutModule.h"
#include "YSFIOStreamMsg.h"
#include "util.h"

namespace YSFIO
{
	std::unique_ptr<YSFIOTimerTaskProc> YSFIOTimerTaskProc::proc = nullptr;
	YSFIOTimerTaskProc::YSFIOTimerTaskProc() :
		m_pos{ 0 }
	{
		for (size_t i = 0; i < TimerQueryLen; i++)
		{
			std::list<IYSFIOTask*> lTask;
			m_lTask.push_back(lTask);
		}
	}
	YSFIOTimerTaskProc::~YSFIOTimerTaskProc()
	{
		for (auto& tasks : m_lTask)
		{
			for (auto& task : tasks)
			{
				if (nullptr != task)
				{
					delete task;
				}
				task = nullptr;
			}
		}
		m_lTask.clear();
	}
	bool YSFIOTimerTaskProc::Init()
	{
		if (nullptr == proc)
		{
			proc.reset(new YSFIOTimerTaskProc);
		}
		return true;
	}
	void YSFIOTimerTaskProc::Fini()
	{
		proc.reset();
	}
	YSFIOTimerTaskProc& YSFIOTimerTaskProc::GetTaskProc()
	{
		return *proc;
	}
	bool YSFIOTimerTaskProc::AddTask(IYSFIOTask* _task)
	{
		if (nullptr == _task)
		{
			return false;
		}
		int sec = _task->TimerSec();
		if (0 >= sec)
		{
			return false;
		}
		_task->m_pos = (proc->m_pos + sec) % TimerQueryLen;
		_task->m_rounds = sec / TimerQueryLen;
		proc->m_lTask.at(_task->m_pos).push_back(_task);
		return true;
	}
	void YSFIOTimerTaskProc::DelTask(IYSFIOTask* _task)
	{
		if (nullptr == _task)
		{
			return;
		}
		_task->KillTask();
	}
	YSFIOStreamMsg* YSFIOTimerTaskProc::InternelHandle(YSFIOStreamMsg& _msg)
	{
		GET_REF2DATA(BytesMsg, oBytes, _msg);
		uint64_t timerCount = 0;
		oBytes.bytesData.copy((char*)&timerCount, sizeof(timerCount), 0);
		/* 任务处理单独抽离，防止任务处理过长，任务添加错乱 */
		std::list<IYSFIOTask*> needProcTask;
		while (timerCount-- > 0)
		{
			m_pos %= TimerQueryLen;
			/* 暂存超时任务 */
			std::list<IYSFIOTask*> tasks;
			for (auto& task : m_lTask.at(m_pos))
			{
				if (0 >= task->m_rounds)
				{
					tasks.push_back(task);
					needProcTask.push_back(task);
					continue;
				}
				task->m_rounds--;
			}
			/* 从原队列移至新队列，或移除 */
			for (auto& task : tasks)
			{
				m_lTask.at(m_pos).remove(task);
				if (!task->IsLive())
				{
					needProcTask.remove(task);
					delete task;
					task = nullptr;
				}
				else
				{
					AddTask(task);
				}
			}
			++m_pos;
		}
		/* 任务处理单独抽离，防止任务处理过长，任务添加错乱 */
		/* 处理所有触发任务 */
		for (auto& task : needProcTask)
		{
			task->Proc();
		}
		return nullptr;
	}
	AYSFIOHandle* YSFIOTimerTaskProc::GetNext(YSFIOStreamMsg& _msg)
	{
		return nullptr;
	}
	YSFIOTimerMngChannel::YSFIOTimerMngChannel() :
		m_timerFd{ -1 }
	{
	}
	YSFIOTimerMngChannel::~YSFIOTimerMngChannel()
	{
		if (0 <= m_timerFd)
		{
			close(m_timerFd);
		}
	}
	bool YSFIOTimerMngChannel::Init()
	{
		bool bRet = false;
		do
		{
			if (!YSFIOTimerTaskProc::Init())
			{
				break;
			}
			int fd = timerfd_create(CLOCK_MONOTONIC, 0);
			if (0 > fd)
			{
				LOG("timerfd_create error");
				break;
			}
			itimerspec timer = { {1,0},{1,0} };
			if (0 > timerfd_settime(fd, 0, &timer, nullptr))
			{
				LOG("timerfd_settime error");
				close(fd);
				break;
			}
			m_timerFd = fd;
			bRet = true;
		} while (false);
		return bRet;
	}
	void YSFIOTimerMngChannel::Fini()
	{
		if (0 <= m_timerFd)
		{
			close(m_timerFd);
		}
		YSFIOTimerTaskProc::Fini();
	}
	bool YSFIOTimerMngChannel::ReadFd(std::string& _input)
	{
		char count[8] = { 0 };
		if (sizeof(count) != read(m_timerFd, count, sizeof(count)))
		{
			return false;
		}
		_input.assign(count, sizeof(count));
		return true;
	}
	bool YSFIOTimerMngChannel::WriteFd(std::string& _output)
	{
		return false;
	}
	int YSFIOTimerMngChannel::GetFd()
	{
		return m_timerFd;
	}
	std::string YSFIOTimerMngChannel::GetChannelInfo()
	{
		return "YSFIOTimerMngChannel";
	}
	AYSFIOHandle* YSFIOTimerMngChannel::GetInputNextStage(BytesMsg& _input)
	{
		return &YSFIOTimerTaskProc::GetTaskProc();
	}
	IYSFIOTask::IYSFIOTask() :
		m_pos{ -1 },
		m_rounds{ -1 },
		m_isLive{ true }
	{
	}
}