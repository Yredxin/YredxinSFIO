#pragma once
#include <string>
#include <memory>
#include "../YSFIO/IYSFIOChannel.h"
#include "TestOut.h"

class TestIn :
	public YSFIO::IYSFIOChannel
{
	std::shared_ptr<TestOut> out;
public:
	virtual bool ReadFd(YSFIO::ByteMsg& _input) override;
	virtual bool WriteFd(YSFIO::ByteMsg& _output) override;
	virtual std::string GetChannelInfo() override;
	virtual int GetFd() override;
	void AddOut(std::shared_ptr<TestOut>& _out);

	// 通过 IYSFIOChannel 继承
	virtual std::shared_ptr<YSFIO::IYSFIOChannel> GetInputNextStage(std::shared_ptr<YSFIO::ByteMsg> _msg) override;
};

