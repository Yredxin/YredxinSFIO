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
	cout << _output << endl;
	return true;
}
