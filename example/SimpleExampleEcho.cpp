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
#include "../YSFIO/YSFIOKernel.h"
#include "../YSFIO/IYSFIOChannel.h"
#include "../YSFIO/AYSFIOHandle.h"
#include "../YSFIO/YSFIOStreamMsg.h"
#include "../YSFIO/util.h"

class TestOut :
	public YSFIO::IYSFIOChannel
{
public:

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
	virtual int GetFd() override
	{
		return 1;
	}
	virtual std::string GetChannelInfo() override
	{
		return "stdout";
	}
	virtual AYSFIOHandle* GetInputNextStage(YSFIO::BytesMsg& _input) override
	{
		return nullptr;
	}
}*out = new TestOut{};

class Echo :
	public YSFIO::AYSFIOHandle
{
public:

	// 通过 AYSFIOHandle 继承
	virtual YSFIO::YSFIOStreamMsg* InternelHandle(YSFIO::YSFIOStreamMsg& _msg) override
	{
		GET_REF2DATA(YSFIO::BytesMsg, oBytes, _msg);
		YSFIO::YSFIOKernel::SendOut(oBytes.bytesData, out);
		return nullptr;
	}
	virtual AYSFIOHandle* GetNext(YSFIO::YSFIOStreamMsg& _msg) override
	{
		return nullptr;
	}
}*echo = new Echo{};

class TestIn :
	public YSFIO::IYSFIOChannel
{
public:

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
	virtual int GetFd() override
	{
		return 0;
	}
	virtual std::string GetChannelInfo() override
	{
		return "stdin";
	}
	virtual AYSFIOHandle* GetInputNextStage(YSFIO::BytesMsg& _input) override
	{
		return echo;
	}
}*in = new TestIn{};

#if 0
int main()
{
	using namespace YSFIO;

	YSFIOKernel::Init();
	YSFIOKernel::AddChannel(out);
	YSFIOKernel::AddChannel(in);
	YSFIOKernel::Run();
	YSFIOKernel::Fini();
	delete echo;
	echo = nullptr;
	return 0;
}
#endif