/*************************************************************************************
 *
 * 文 件 名:	SimpleExampleTimerTask.cpp
 * 描    述:	时间轮demo，每2秒打印一次hello，10秒后程序结束
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/10/3 9:49
 *
*************************************************************************************/
#include <iostream>
#include "../YSFIO/YSFIOKernel.h"
#include "../YSFIO/YSFIOTimeoutModule.h"

class MyTask :
	public YSFIO::IYSFIOTask
{
public:

	// 通过 IYSFIOTask 继承
	virtual void Proc() override
	{
		std::cout << "hello" << std::endl;
	}
	virtual int TimerSec() override
	{
		return 2;
	}
}*myTask = new MyTask;


class OverHelloTask :
	public YSFIO::IYSFIOTask
{
public:

	// 通过 IYSFIOTask 继承
	virtual void Proc() override
	{
		YSFIO::YSFIOTimerTaskProc::DelTask(myTask);
	}
	virtual int TimerSec() override
	{
		return 5;
	}
};


class MyQuit :
	public YSFIO::IYSFIOTask
{
public:

	// 通过 IYSFIOTask 继承
	virtual void Proc() override
	{
		YSFIO::YSFIOKernel::Exit();
	}
	virtual int TimerSec() override
	{
		return 10;
	}
};

int main()
{
	YSFIO::YSFIOKernel::Init();
	YSFIO::YSFIOKernel::AddChannel(new YSFIO::YSFIOTimerMngChannel());
	YSFIO::YSFIOTimerTaskProc::AddTask(myTask);
	YSFIO::YSFIOTimerTaskProc::AddTask(new OverHelloTask);
	YSFIO::YSFIOTimerTaskProc::AddTask(new MyQuit);
	YSFIO::YSFIOKernel::Run();
	YSFIO::YSFIOKernel::Fini();
	return 0;
}