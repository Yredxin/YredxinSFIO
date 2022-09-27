#include "TestIn.h"
#include <iostream>

using namespace std;
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

void TestIn::AddOut(TestOut* _out)
{
	out = _out;
}

void TestIn::Business(std::string& _msg)
{
	out->WriteFd(_msg);
}
