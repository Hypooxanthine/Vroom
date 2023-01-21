#pragma once

#include <functional>

namespace Vroom::Event
{
	using Callback = std::function<void(bool)>;
}