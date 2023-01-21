#include "Vroom/Events/Event.h"

namespace Vroom::Event
{
	void Event::trigger(bool value)
	{
		if (value)
		{
			m_Triggers++;
			if (m_Triggers == 1) m_Callbacks.triggerAll(true);
		}
		else if (m_Triggers > 0)
		{
			m_Triggers--;
			if (m_Triggers == 0) m_Callbacks.triggerAll(false);
		}
	}

	void Event::notifyFocusLost()
	{
		if (isTriggered())
			m_Callbacks.triggerAll(false);

		m_Triggers = 0;
	}
}