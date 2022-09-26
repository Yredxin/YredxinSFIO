﻿#include <iostream>
#include <memory>
#include "../YSFIO/YSFIOKernel.h"
#include "TestIn.h"
using namespace std;

int main(void)
{
	auto& kernel = YSFIO::YSFIOKernel::GetInstance();
	kernel.Init();
	kernel.AddTestIn(make_shared<TestIn>());
	kernel.Run();
	kernel.Fini();
	return 0;
}