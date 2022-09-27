#include "AYSFIOHandle.h"

void YSFIO::AYSFIOHandle::Handle(std::shared_ptr<AYSFIOMsg> _msg)
{
	if (nullptr == _msg)
	{
		return;
	}
	auto msg = InternelHandle(_msg);
	if (nullptr == msg)
	{
		return;
	}
	auto next = GetNext(msg);
	if (nullptr == next)
	{
		return;
	}
	next->Handle(msg);
}

YSFIO::AYSFIOHandle::~AYSFIOHandle()
{
}
