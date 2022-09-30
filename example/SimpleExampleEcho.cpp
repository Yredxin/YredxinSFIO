/*************************************************************************************
 *
 * 文 件 名:	SimpleExampleEcho.cpp
 * 描    述:	回显例子
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/9/30 11:23
 *
*************************************************************************************/
#include <iostream>
#include "../YSFIO/IYSFIOHandle.h"
#include "../YSFIO/YSFIOKernel.h"
#include "../YSFIO/IYSFIOChannel.h"
class TestOut :
	public YSFIO::IYSFIOChannel
{
public:
	TestOut() = default;
	virtual ~TestOut() = default;
	// 通过 IYSFIOChannel 继承
	virtual bool Init() override
	{
		return true;
	}

	virtual void Fini() override
	{
	}

	virtual bool ReadFd(std::string& _input) override
	{
		return false;
	}

	virtual bool WriteFd(std::string& _output) override
	{
		std::cout << _output << std::endl;
		return true;
	}

	virtual std::string GetChannelInfo() override
	{
		return "testout";
	}

	virtual int GetFd() override
	{
		return 1;
	}

	virtual AYSFIOHandle* GetInputNextStage(YSFIO::BytesMsg& _msg) override
	{
		return nullptr;
	}

}*out = new TestOut{};

class Echo :
	public YSFIO::IYSFIOHandle
{
public:

	Echo() = default;
	virtual ~Echo() = default;
	// 通过 IYSFIOHandle 继承
	virtual YSFIO::BytesMsg* ProcService(YSFIO::BytesMsg& _msg) override
	{
		YSFIO::YSFIOKernel::YSFIO_SendOut(_msg.msgData, *out);
		return nullptr;
	}
	virtual AYSFIOHandle* GetNextHandler(YSFIO::BytesMsg& _msg) override
	{
		return nullptr;
	}
};
std::shared_ptr<Echo> echo{ new Echo };

class TestIn :
	public YSFIO::IYSFIOChannel
{
public:
	TestIn() = default;
	virtual ~TestIn() = default;
	// 通过 IYSFIOChannel 继承
	virtual bool Init() override
	{
		return true;
	}

	virtual void Fini() override
	{
	}

	virtual bool ReadFd(std::string& _input) override
	{
		std::cin >> _input;
		return true;
	}

	virtual bool WriteFd(std::string& _output) override
	{
		return false;
	}

	virtual std::string GetChannelInfo() override
	{
		return "testin";
	}

	virtual int GetFd() override
	{
		return 0;
	}

	virtual AYSFIOHandle* GetInputNextStage(YSFIO::BytesMsg& _msg) override
	{
		return echo.get();
	}

}*in = new TestIn{};

int main()
{
	YSFIO::YSFIOKernel::YSFIO_Init();
	YSFIO::YSFIOKernel::YSFIO_AddChannel(in);
	YSFIO::YSFIOKernel::YSFIO_AddChannel(out);
	YSFIO::YSFIOKernel::YSFIO_Run();
	YSFIO::YSFIOKernel::YSFIO_Fini();
	return 0;
}