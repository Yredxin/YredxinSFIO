/*************************************************************************************
 *
 * 文 件 名:	IYSFIORole.h
 * 描    述:
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/9/30 10:51
 *
*************************************************************************************/
#ifndef _YSFIO_IROLE_
#define _YSFIO_IROLE_
#include <memory>
#include "AYSFIOHandle.h"

namespace YSFIO
{
	class IYSFIOProtocol;
	struct UserData;

	class IYSFIORole :
		public YSFIO::AYSFIOHandle
	{
	public:
		IYSFIORole() = default;
		virtual ~IYSFIORole() = default;

		virtual bool Init() = 0;
		virtual void Fini() = 0;
		/* 消息堆对象 */
		virtual UserData* ProcMsg(UserData& _userData) = 0;
		/* 设置下一个处理者 */
		void SetNextProcessor(IYSFIORole* _nextProc);
		/* 清除下一个处理者 */
		void ClearNextProcessor();
	private:
		std::shared_ptr<IYSFIORole> pNextProc = nullptr;
		// 通过 AYSFIOHandle 继承
		virtual std::unique_ptr<YSFIOMsg> InternelHandle(YSFIOMsg& _msg) override;
		virtual std::shared_ptr<AYSFIOHandle> GetNext(YSFIOMsg& _msg) override;

	};
}

#endif // !_YSFIO_IROLE_

