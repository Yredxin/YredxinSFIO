#include "TestIn.h"
#include <iostream>
#include <unistd.h>
#include "../YSFIO/YSFIOKernel.h"

using namespace std;

void TestIn::AddStdOut(std::shared_ptr<TestOut>& _out)
{
	out = _out;
}

bool TestIn::Init()
{
	return true;
}

void TestIn::Fini()
{
}

bool TestIn::ReadFd(std::string& _input)
{
	cin >> _input;
	return true;
}

bool TestIn::WriteFd(std::string& _output)
{
	return false;
}

std::string TestIn::GetChannelInfo()
{
	return std::string();
}

int TestIn::GetFd()
{
	return 0;
}

YSFIO::IYSFIOChannel* TestIn::GetInputNextStage(YSFIO::BytesMsg& _msg)
{
	YSFIO::YSFIOKernel::YSFIO_SendOut(_msg.msgData, out);
	return nullptr;
}
