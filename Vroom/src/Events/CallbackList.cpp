#include "Vroom/Events/CallbackList.h"

namespace Vroom::Event
{
	void CallbackList::triggerAll(bool value)
	{
		for (auto& cb : m_List)
			cb(value);
	}
}