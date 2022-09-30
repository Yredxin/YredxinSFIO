/*************************************************************************************
 *
 * 文 件 名:	main.cpp
 * 描    述:	测试文件
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/9/29 9:08
 *
*************************************************************************************/
#include <iostream>
#include <memory>
#include "../YSFIO/YSFIOKernel.h"
#include "TestIn.h"
#include "TestOut.h"
#include "../YSFIO/util.h"
#include <sys/epoll.h>
using namespace std;

int main(void)
{
	YSFIO::YSFIOKernel::YSFIO_Init();
	shared_ptr<TestIn> in = make_shared<TestIn>();
	shared_ptr<TestOut> out = make_shared<TestOut>();
	in->AddStdOut(out);
	YSFIO::YSFIOKernel::YSFIO_AddChannel(in);
	YSFIO::YSFIOKernel::YSFIO_AddChannel(out);
	YSFIO::YSFIOKernel::YSFIO_Run();
	YSFIO::YSFIOKernel::YSFIO_Fini();
	return 0;
}