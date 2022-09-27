#pragma once
#include "../YSFIO/IYSFIOChannel.h"
class TestOut :
	public YSFIO::IYSFIOChannel
{
public:
	// 通过 IYSFIOChannel 继承
	virtual bool ReadFd(std::string& _input) override;
	virtual bool WriteFd(std::string& _output) override;
	virtual std::string GetChannelInfo() override;
	virtual int GetFd() override;
};

