/*************************************************************************************
 *
 * 文 件 名:	YSFIOTcpModule.h
 * 描    述:	TCP通信模块，服务器TCP
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/10/2 16:52
 *
*************************************************************************************/
#ifndef _YSFIO_TCP_MODULE
#define _YSFIO_TCP_MODULE
#include <memory>
#include "IYSFIOChannel.h"

namespace YSFIO
{

	/* 处理连接对象 */
	class YSFIOTcpData :
		public YSFIO::IYSFIOChannel
	{
		int m_fd;
	public:
		YSFIOTcpData(int _fd);
		virtual ~YSFIOTcpData();
		// 通过 IYSFIOChannel 继承
		virtual bool Init() override;

		virtual void Fini() override;

		virtual bool ReadFd(std::string& _input) override;

		virtual bool WriteFd(std::string& _output) override;

		virtual int GetFd() override;

		virtual std::string GetChannelInfo() override;

		virtual AYSFIOHandle* GetInputNextStage(BytesMsg& _input) override = 0;

	};

	/* TcpData工厂类 */
	class IYSFIOTcpFactory
	{
	public:
		virtual YSFIOTcpData* CreateTcpDataChannel(int _fd) = 0;
	};

	/* 开启监听 */
	class YSFIOTcpListen :
		public YSFIO::IYSFIOChannel
	{
		int m_port;
		int m_listenFd;
		std::unique_ptr<IYSFIOTcpFactory> m_factory;
	public:
		YSFIOTcpListen(int _port, IYSFIOTcpFactory* _factory);
		~YSFIOTcpListen();
		// 通过 IYSFIOChannel 继承
		virtual bool Init() override;
		virtual void Fini() override;
		virtual bool ReadFd(std::string& _input) override;
		virtual bool WriteFd(std::string& _output) override;
		virtual int GetFd() override;
		virtual std::string GetChannelInfo() override;
		virtual AYSFIOHandle* GetInputNextStage(BytesMsg& _input) override;
	};
}

#endif // !_YSFIO_TCP_MODULE
