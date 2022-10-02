/*************************************************************************************
 *
 * 文 件 名:	SimpleExampleTcp.cpp
 * 描    述:
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/10/2 18:18
 *
*************************************************************************************/

/*************************************************************************************
 *
 * 文 件 名:	SimpleExampleTcp.cpp
 * 描    述:
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/10/2 18:18
 *
*************************************************************************************/
#include <iostream>
#include <list>
#include "../YSFIO/IYSFIOChannel.h"
#include "../YSFIO/YSFIOKernel.h"
#include "../YSFIO/AYSFIOHandle.h"
#include "../YSFIO/YSFIOTcpModule.h"
#include "../YSFIO/YSFIOStreamMsg.h"
#include "../YSFIO/util.h"

class MyEcho :
	public YSFIO::AYSFIOHandle
{
public:
	// 通过 AYSFIOHandle 继承
	virtual YSFIO::YSFIOStreamMsg* InternelHandle(YSFIO::YSFIOStreamMsg& _msg) override
	{
		GET_REF2DATA(YSFIO::BytesMsg, oBytes, _msg);
		auto channel = YSFIO::YSFIOKernel::GetFirstInfoChannel("YSFIOTcpData");
		YSFIO::YSFIOKernel::SendOut(oBytes.bytesData, channel);
		return nullptr;
	}
	virtual AYSFIOHandle* GetNext(YSFIO::YSFIOStreamMsg& _msg) override
	{
		return nullptr;
	}
}*echo2 = new MyEcho;

class MyTcpData :
	public YSFIO::YSFIOTcpData
{
public:
	MyTcpData(int _fd) :YSFIO::YSFIOTcpData{ _fd } {};
	~MyTcpData() {};
	// 通过 YSFIOTcpData 继承
	virtual AYSFIOHandle* GetInputNextStage(YSFIO::BytesMsg& _input) override
	{
		return echo2;
	}
};

class MyTcpFactory :
	public YSFIO::IYSFIOTcpFactory
{
public:

	// 通过 IYSFIOTcpFactory 继承
	virtual YSFIO::YSFIOTcpData* CreateTcpDataChannel(int _fd) override
	{
		return new MyTcpData{ _fd };
	}
};

int main()
{
	YSFIO::YSFIOKernel::Init();
	YSFIO::YSFIOKernel::AddChannel(new YSFIO::YSFIOTcpListen{ 8888 , new MyTcpFactory });
	YSFIO::YSFIOKernel::Run();
	YSFIO::YSFIOKernel::Fini();
	delete echo2;
	return 0;
}