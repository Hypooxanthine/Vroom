#pragma once

#include <functional>

#include "Event/CallbackList.h"

namespace vrm
{

using TriggerCallback = std::function<void(bool)>;

} // namespace vrm
