#pragma once
#include "../YSFIO/IYSFIOChannel.h"
class TestOut :
	public YSFIO::IYSFIOChannel
{
public:

	// 通过 IYSFIOChannel 继承
	virtual bool Init() override;
	virtual void Fini() override;
	virtual bool ReadFd(std::string& _input) override;
	virtual bool WriteFd(std::string& _output) override;
	virtual std::string GetChannelInfo() override;
	virtual int GetFd() override;
	virtual YSFIO::IYSFIOChannel* GetInputNextStage(YSFIO::BytesMsg& _msg) override;
};

