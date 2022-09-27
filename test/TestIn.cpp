#include "TestIn.h"
#include <iostream>
#include "../YSFIO/YSFIOKernel.h"

using namespace std;
bool TestIn::ReadFd(YSFIO::ByteMsg& _input)
{
	cin >> _input.msgData;
	return true;
}

bool TestIn::WriteFd(YSFIO::ByteMsg& _output)
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

void TestIn::AddOut(std::shared_ptr<TestOut>& _out)
{
	out = _out;
}

std::shared_ptr<YSFIO::IYSFIOChannel> TestIn::GetInputNextStage(std::shared_ptr<YSFIO::ByteMsg> _msg)
{
	YSFIO::YSFIOKernel::GetInstance().SendOut(_msg->msgData, out);
	return nullptr;
}
