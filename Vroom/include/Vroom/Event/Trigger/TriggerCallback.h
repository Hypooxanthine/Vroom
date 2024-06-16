#pragma once

#include <functional>

#include "Vroom/DataStructure/CallbackList.h"

namespace vrm
{

using TriggerCallback = std::function<void(bool)>;

} // namespace vrm
