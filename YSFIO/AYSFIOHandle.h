#ifndef _YSFIO_AHANDLE_
#define _YSFIO_AHANDLE_
#include <memory>

namespace YSFIO
{
	struct YSFIOMsg
	{
		YSFIOMsg() = default;
		virtual ~YSFIOMsg() = default;
	};

	class AYSFIOHandle
	{
	public:
		void Handle(YSFIOMsg& _msg);
		virtual std::unique_ptr<YSFIOMsg> InternelHandle(YSFIOMsg& _msg) = 0;
		virtual std::unique_ptr<AYSFIOHandle> GetNext(YSFIOMsg& _msg) = 0;
		virtual ~AYSFIOHandle();
	};
}

#endif // !_YSFIO_AHANDLE_

