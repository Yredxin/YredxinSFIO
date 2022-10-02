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
#include "AYSFIOHandle.h"

namespace YSFIO
{
	class UserData;

	class IYSFIORole :
		public AYSFIOHandle
	{
	public:
		IYSFIORole();
		virtual ~IYSFIORole();

		/* role初始化 */
		virtual bool Init() = 0;

		/* role销毁 */
		virtual void Fini() = 0;

		/* 重写此函数进行业务处理，如果有后续，则不能返回nullptr */
		virtual UserData* ProcMsg(UserData& _userData) = 0;

		/* 设置下一个处理对象 */
		void SetNextProcessor(IYSFIORole* _role);

		/* 清除下一个处理对象 */
		void ClearNextProcessor();

	private:
		// 通过 AYSFIOHandle 继承
		virtual YSFIOStreamMsg* InternelHandle(YSFIOStreamMsg& _msg) override;
		virtual AYSFIOHandle* GetNext(YSFIOStreamMsg& _msg) override;

		IYSFIORole* m_nextRole;
	};
}

#endif // !_YSFIO_IROLE_

