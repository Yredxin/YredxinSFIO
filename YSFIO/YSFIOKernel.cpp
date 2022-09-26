#include "YSFIOKernel.h"
#include <iostream>
#include <string>
using namespace std;
using namespace YSFIO;

YSFIOKernel YSFIOKernel::m_instance{};

void YSFIOKernel::Run()
{
	while (true)
	{
		string buf;
		in->ReadFd(buf);
		in->WriteFd(buf);
	}
}

YSFIOKernel& YSFIOKernel::GetInstance()
{
	return m_instance;
}

bool YSFIO::YSFIOKernel::AddTestIn(std::shared_ptr<TestIn> _in)
{
	in = _in;
	return true;;
}

YSFIOKernel::YSFIOKernel()
{
}

YSFIOKernel::~YSFIOKernel()
{
}
