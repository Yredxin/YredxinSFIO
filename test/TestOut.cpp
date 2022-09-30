/*************************************************************************************
 *
 * 文 件 名:	TestOut.cpp
 * 描    述:	输出测试实现
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/9/29 9:08
 *
*************************************************************************************/
#include "TestOut.h"
#include <iostream>
#include <unistd.h>
using namespace std;

bool TestOut::Init()
{
	return true;
}

void TestOut::Fini()
{
}

bool TestOut::ReadFd(std::string& _input)
{
	return false;
}

bool TestOut::WriteFd(std::string& _output)
{
	cout << _output << endl;
	return true;
}

std::string TestOut::GetChannelInfo()
{
	return std::string();
}

int TestOut::GetFd()
{
	return 1;
}

YSFIO::IYSFIOChannel* TestOut::GetInputNextStage(YSFIO::BytesMsg& _msg)
{
	return nullptr;
}
