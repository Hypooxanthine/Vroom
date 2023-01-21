#pragma once

#include <unordered_map>

#include "Vroom/Events/CallbackList.h"

namespace Vroom::Event
{
	class Event
	{
	public:
		Event() = default;

		inline void bindCallback(const Callback& cb) { m_Callbacks.addCallback(cb); }

		void trigger(bool value);

		void notifyFocusLost();

		inline bool isTriggered() const { return m_Triggers > 0; }

	private:
		// The number of triggers currently used
		uint8_t m_Triggers = 0;

		// Callbacks of the event.
		CallbackList m_Callbacks;
	};
}