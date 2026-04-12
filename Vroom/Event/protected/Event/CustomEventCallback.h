#pragma once

#include <functional>

#include "Event/Event.h"

namespace vrm
{

using CustomEventCallback = std::function<void(const Event& e)>;

} // namespace vrm
