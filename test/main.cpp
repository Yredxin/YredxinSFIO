#include <iostream>
#include "../YSFIO/YSFIOKernel.h"
using namespace std;

int main(void)
{
	auto& kernel = YSFIO::YSFIOKernel::GetInstance();
	kernel.Run();
	return 0;
}