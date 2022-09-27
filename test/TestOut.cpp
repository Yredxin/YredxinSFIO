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
	cout << _output << endl;
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
