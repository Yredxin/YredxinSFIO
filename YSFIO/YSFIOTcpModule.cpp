/*************************************************************************************
 *
 * 文 件 名:	YSFIOTcpModule.cpp
 * 描    述:
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/10/2 16:52
 *
*************************************************************************************/
#include <sys/types.h>        
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

#include "YSFIOTcpModule.h"
#include "YSFIOKernel.h"
#include "util.h"

namespace YSFIO
{
	YSFIOTcpData::YSFIOTcpData(int _fd) :
		m_fd{ _fd }
	{
	}
	YSFIOTcpData::~YSFIOTcpData()
	{
		if (-1 != m_fd)
		{
			close(m_fd);
		}
	}
	bool YSFIOTcpData::Init()
	{
		return true;
	}
	void YSFIOTcpData::Fini()
	{
	}
	bool YSFIOTcpData::ReadFd(std::string& _input)
	{
		bool bRet = false;
		char buf[1024] = { 0 };
		int len = -1;
		/* MSG_DONTWAIT设置读取非阻塞 */
		while (0 < (len = recv(m_fd, buf, sizeof(buf), MSG_DONTWAIT)))
		{
			bRet = true;
			_input.append(buf, len);
		}


		if (!bRet)
		{
			SetNeedClose();
		}
#ifdef DEBUG
		if (bRet)
		{
			std::cout << "<----------------------------------------->" << std::endl;
			std::cout << "recv from " << m_fd << ":" << IYSFIOChannel::Convert2Printable(_input) << std::endl;
			std::cout << "<----------------------------------------->" << std::endl;
		}
		else
		{
			std::cout << "<----------------------------------------->" << std::endl;
			std::cout << m_fd << ": close" << std::endl;
			std::cout << "<----------------------------------------->" << std::endl;
		}
#endif
		return bRet;
	}
	bool YSFIOTcpData::WriteFd(std::string& _output)
	{
		bool bRet = false;

		if (_output.size() == send(m_fd, _output.c_str(), _output.size(), MSG_DONTWAIT))
		{
			bRet = true;
#ifdef DEBUG
			std::cout << "<----------------------------------------->" << std::endl;
			std::cout << "send to " << m_fd << ":" << IYSFIOChannel::Convert2Printable(_output) << std::endl;
			std::cout << "<----------------------------------------->" << std::endl;
#endif
		}
		return bRet;
	}
	int YSFIOTcpData::GetFd()
	{
		return m_fd;
	}
	std::string YSFIOTcpData::GetChannelInfo()
	{
		return "YSFIOTcpData";
	}

	YSFIOTcpListen::YSFIOTcpListen(int _port, IYSFIOTcpFactory* _factory) :
		m_port{ _port },
		m_listenFd{ -1 },
		m_factory{ _factory }
	{
	}
	YSFIOTcpListen::~YSFIOTcpListen()
	{
		Fini();
	}
	bool YSFIOTcpListen::Init()
	{
		bool bRet = false;
		do
		{
			if (0 > m_port || 65535 < m_port)
			{
				LOG("端口范围错误");
				break;
			}
			/* 创建套接字 */
			int fd = m_listenFd = socket(AF_INET, SOCK_STREAM, 0);
			if (0 > fd)
			{
				LOG("socket create error");
				break;
			}
			/* 设置端口复用 */
			int opt = 1;
			setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
			/* 绑定端口*/
			sockaddr_in addr;
			addr.sin_family = AF_INET;
			addr.sin_port = htons(m_port);
			addr.sin_addr.s_addr = INADDR_ANY;
			if (0 > bind(fd, (sockaddr*)&addr, sizeof(addr)))
			{
				LOG("socket bind error");
				break;
			}
			if (0 > listen(fd, 10))
			{
				LOG("socket listen error");
				break;
			}
			m_listenFd = fd;
			bRet = true;
		} while (false);
		return bRet;
	}
	void YSFIOTcpListen::Fini()
	{
		if (0 <= m_listenFd)
		{
			close(m_listenFd);
			m_listenFd = -1;
		}
		if (nullptr != m_factory)
		{
			m_factory.reset();
		}
	}
	bool YSFIOTcpListen::ReadFd(std::string& _input)
	{
		/* 有新连接 */
		sockaddr_in addr;
		socklen_t len = sizeof(addr);
		int comFd = accept(m_listenFd, (sockaddr*)&addr, &len);
		if (0 > comFd)
		{
			LOG("accept connect error");
			return false;
		}
		YSFIOKernel::AddChannel(m_factory->CreateTcpDataChannel(comFd));
		return true;
	}
	bool YSFIOTcpListen::WriteFd(std::string& _output)
	{
		return false;
	}
	int YSFIOTcpListen::GetFd()
	{
		return m_listenFd;
	}
	std::string YSFIOTcpListen::GetChannelInfo()
	{
		return "YSFIOTcpListen";
	}
	AYSFIOHandle* YSFIOTcpListen::GetInputNextStage(BytesMsg& _input)
	{
		return nullptr;
	}
}