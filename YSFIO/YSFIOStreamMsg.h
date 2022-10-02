/*************************************************************************************
 *
 * 文 件 名:	YSFIOStreamMsg.h
 * 描    述:	框架消息传输类
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/10/1 17:13
 *
*************************************************************************************/
#ifndef _YSFIO_STREAMMSG_
#define _YSFIO_STREAMMSG_
#include <string>

namespace YSFIO
{
	/* 框架消息基类 */
	struct YSFIOStreamMsg
	{
		YSFIOStreamMsg() = default;
		virtual ~YSFIOStreamMsg() = default;
	};

	/* 框架内部消息流通类 */
	struct YSFIOFrameMsg :
		public YSFIOStreamMsg
	{
		enum class FrameMsgType
		{
			IN,		/* 表示此条消息为输入 */
			OUT		/* 表示此条消息为输出 */
		}type;
		YSFIOFrameMsg(const FrameMsgType& _type);
		virtual ~YSFIOFrameMsg() = default;
	};

	/* 框架主要数据传输类 */
	struct BytesMsg :
		public YSFIOFrameMsg
	{
		BytesMsg(const YSFIOFrameMsg& _base);
		virtual ~BytesMsg() = default;
		std::string bytesData;
		std::string channelInfo;
	};

	/* 用户自定义数据基类 */
	class UserData
	{
	public:
		UserData() = default;
		virtual ~UserData() = default;
	};

	/* 用户信息类 */
	struct UserMsg :
		public BytesMsg
	{
		UserMsg(const BytesMsg& _base);
		virtual ~UserMsg();
		UserData* userData;
	};
}

#endif // !_YSFIO_STREAMMSG_

