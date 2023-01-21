#pragma once

#include <vector>

#include "Vroom/Events/Callback.h"

namespace Vroom::Event
{
	class CallbackList
	{
	public:
		CallbackList() = default;

		inline void addCallback(const Callback& cb) { m_List.push_back(cb); }
		void triggerAll(bool value);

	private:
		std::vector<Callback> m_List;
	};
}