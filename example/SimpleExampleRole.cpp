/*************************************************************************************
 *
 * 文 件 名:	SimpleExampleRole.cpp
 * 描    述:
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/10/2 9:30
 *
*************************************************************************************/
#include <iostream>
#include <string>
#include "../YSFIO/YSFIOKernel.h"
#include "../YSFIO/IYSFIOChannel.h"
#include "../YSFIO/IYSFIOProtocol.h"
#include "../YSFIO/IYSFIORole.h"
#include "../YSFIO/YSFIOStreamMsg.h"
#include "../YSFIO/util.h"

using namespace YSFIO;

class MyData :
	public YSFIO::UserData
{
public:
	std::string info;
	std::string name;
	std::string sex;
};

class Role :
	public YSFIO::IYSFIORole
{
public:
	IYSFIOProtocol* m_proto;
	// 通过 IYSFIORole 继承
	virtual bool Init() override
	{
		return true;
	}
	virtual void Fini() override
	{
	}
	virtual UserData* ProcMsg(UserData& _userData) override
	{
		GET_REF2DATA(MyData, oMyData, _userData);
		if (oMyData.info == "exit")
		{
			YSFIOKernel::Exit();
		}
		else
		{
			if (oMyData.info.empty())
			{
				oMyData.info = "你好" + oMyData.name + ((oMyData.sex == "男") ? "先生" : "女士") + "！";
			}
			else
			{
				oMyData.info = "输入格式：姓名|性别";
			}
			YSFIOKernel::SendOut(oMyData, m_proto);
		}
		return nullptr;
	}
};

class Proto :
	public YSFIO::IYSFIOProtocol
{
public:
	YSFIO::IYSFIOChannel* m_out;
	YSFIO::IYSFIORole* m_in;
	// 通过 IYSFIOProtocol 继承
	virtual UserData* Raw2Request(std::string& _input) override
	{
		UserData* retData = nullptr;
		if (!_input.empty())
		{
			MyData* data = new MyData;
			int pos = _input.find("|");
			if (0 <= pos)
			{
				data->name = _input.substr(0, pos);
				data->sex = _input.substr(pos + 1, _input.size() - pos - 1);
			}
			else
			{
				data->info = _input;
			}
			retData = data;
			data = nullptr;
		}
		return retData;
	}
	virtual std::string Response2Raw(UserData& _userData) override
	{
		GET_REF2DATA(MyData, oMyData, _userData);
		return oMyData.info;
	}
	virtual IYSFIORole* GetProcMsg(UserMsg& _msg) override
	{
		return m_in;
	}
	virtual IYSFIOChannel* GetSender(BytesMsg& _msg) override
	{
		return m_out;
	}
};

class Channel :
	public YSFIO::IYSFIOChannel
{
public:
	IYSFIOProtocol* m_proto;
	// 通过 IYSFIOChannel 继承
	virtual bool Init() override
	{
		return true;
	}
	virtual void Fini() override
	{
	}
	virtual bool ReadFd(std::string& _input) override
	{
		std::cin >> _input;
		return true;
	}
	virtual bool WriteFd(std::string& _output) override
	{
		std::cout << _output << std::endl;
		return true;
	}
	virtual int GetFd() override
	{
		return 0;
	}
	virtual std::string GetChannelInfo() override
	{
		return "channel";
	}
	virtual AYSFIOHandle* GetInputNextStage(BytesMsg& _input) override
	{
		return m_proto;
	}
};

int main()
{
	Role* role = new Role;
	Proto* proto = new Proto;
	Channel* channel = new Channel;
	proto->m_in = role;
	proto->m_out = channel;
	channel->m_proto = proto;
	role->m_proto = proto;
	YSFIOKernel::Init();
	YSFIOKernel::AddChannel(channel);
	YSFIOKernel::AddProtocol(proto);
	YSFIOKernel::AddRole(role);
	YSFIOKernel::Run();
	YSFIOKernel::Fini();
	return 0;
}