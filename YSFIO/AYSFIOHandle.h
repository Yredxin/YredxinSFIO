#ifndef _YSFIO_AHANDLE_
#define _YSFIO_AHANDLE_
#include <memory>

namespace YSFIO
{
	struct AYSFIOMsg
	{
		enum class MsgType
		{
			MSG_IN,
			MSG_OUT
		}type;
		AYSFIOMsg(MsgType _type) :type{ _type } {};
		virtual ~AYSFIOMsg() {};
	};

	class AYSFIOHandle
	{
	public:
		void Handle(std::shared_ptr<AYSFIOMsg> _msg);
		virtual std::shared_ptr<AYSFIOMsg> InternelHandle(std::shared_ptr<AYSFIOMsg> _msg) = 0;
		virtual std::shared_ptr<AYSFIOHandle> GetNext(std::shared_ptr<AYSFIOMsg> _msg) = 0;
		virtual ~AYSFIOHandle();
	};
}

#endif // !_YSFIO_AHANDLE_

