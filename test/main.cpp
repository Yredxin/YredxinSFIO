#include <iostream>
#include <memory>
#include "../YSFIO/YSFIOKernel.h"
#include "TestIn.h"
#include "TestOut.h"
#include "../YSFIO/util.h"
using namespace std;

int main(void)
{
	auto& kernel = YSFIO::YSFIOKernel::GetInstance();
	kernel.Init();
	auto in = make_shared<TestIn>();
	auto out = make_shared<TestOut>();
	in->AddOut(out);
	kernel.AddIChannel(in);
	kernel.AddIChannel(out);
	kernel.Run();
	kernel.Fini();
	return 0;
}