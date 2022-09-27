#include "TestOut.h"
#include <iostream>
#include <unistd.h>
using namespace std;

bool TestOut::ReadFd(std::string& _input)
{
	return false;
}

bool TestOut::WriteFd(std::string& _output)
{
	SendMsg(_output);
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

void TestOut::Business(std::string& _msg)
{
}
