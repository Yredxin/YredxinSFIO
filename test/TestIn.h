#pragma once
#include <string>

class TestIn
{
public:
	bool ReadFd(std::string& _input);
	bool WriteFd(std::string& _output);
};

