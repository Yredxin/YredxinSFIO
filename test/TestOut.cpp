#include "TestOut.h"
#include <iostream>
#include <unistd.h>
using namespace std;

bool TestOut::ReadFd(YSFIO::ByteMsg& _input)
{
	return false;
}

bool TestOut::WriteFd(YSFIO::ByteMsg& _output)
{
	cout << _output.msgData << endl;
	return true;
}

std::string TestOut::GetChannelInfo()
{
	return std::string();
}

int TestOut::GetFd()
{
	return STDOUT_FILENO;
}

std::shared_ptr<YSFIO::IYSFIOChannel> TestOut::GetInputNextStage(std::shared_ptr<YSFIO::ByteMsg> _msg)
{
	return nullptr;
}
