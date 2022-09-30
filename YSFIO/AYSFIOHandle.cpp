#include "AYSFIOHandle.h"

void YSFIO::AYSFIOHandle::Handle(YSFIOMsg& _msg)
{
	auto msg = InternelHandle(_msg);
	if (nullptr == msg)
	{
		return;
	}
	auto next = GetNext(*msg);
	if (nullptr == next)
	{
		return;
	}
	next->Handle(*msg);
}

YSFIO::AYSFIOHandle::~AYSFIOHandle()
{
}
