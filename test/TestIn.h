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
	void AddStdOut(std::shared_ptr<TestOut>& _out);
	// 通过 IYSFIOChannel 继承
	virtual bool Init() override;
	virtual void Fini() override;
	virtual bool ReadFd(std::string& _input) override;
	virtual bool WriteFd(std::string& _output) override;
	virtual std::string GetChannelInfo() override;
	virtual int GetFd() override;
	virtual YSFIO::IYSFIOChannel* GetInputNextStage(YSFIO::BytesMsg& _msg) override;
};

