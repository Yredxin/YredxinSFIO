#pragma once
#include <string>
#include "../YSFIO/IYSFIOChannel.h"
#include "TestOut.h"

class TestIn :
	public YSFIO::IYSFIOChannel
{
	TestOut* out;
public:
	virtual bool ReadFd(std::string& _input) override;
	virtual bool WriteFd(std::string& _output) override;
	virtual std::string GetChannelInfo() override;
	virtual int GetFd() override;
	void AddOut(TestOut* _out);
	virtual void Business(std::string& _msg) override;
};

