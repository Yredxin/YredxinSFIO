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
	out->WriteFd(_output);
	return true;
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
