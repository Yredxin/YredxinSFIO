#ifndef _YSFIO_AHANDLE_
#define _YSFIO_AHANDLE_
#include <memory>

namespace YSFIO
{
	struct AYSFIOMsg
	{
		AYSFIOMsg() = default;
		virtual ~AYSFIOMsg() = default;
	};

	class AYSFIOHandle
	{
	public:
		void Handle(AYSFIOMsg& _msg);
		virtual std::unique_ptr<AYSFIOMsg> InternelHandle(AYSFIOMsg& _msg) = 0;
		virtual std::unique_ptr<AYSFIOHandle> GetNext(AYSFIOMsg& _msg) = 0;
		virtual ~AYSFIOHandle();
	};
}

#endif // !_YSFIO_AHANDLE_

