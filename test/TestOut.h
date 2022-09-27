#pragma once
#include "../YSFIO/IYSFIOChannel.h"
class TestOut :
	public YSFIO::IYSFIOChannel
{
public:
	// 通过 IYSFIOChannel 继承
	virtual bool ReadFd(YSFIO::ByteMsg& _input) override;
	virtual bool WriteFd(YSFIO::ByteMsg& _output) override;
	virtual std::string GetChannelInfo() override;
	virtual int GetFd() override;

	// 通过 IYSFIOChannel 继承
	virtual std::shared_ptr<YSFIO::IYSFIOChannel> GetInputNextStage(std::shared_ptr<YSFIO::ByteMsg> _msg) override;
};

