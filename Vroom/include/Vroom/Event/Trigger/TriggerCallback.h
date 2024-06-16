#pragma once

#include "Vroom/DataStructure/CallbackList.h"
#include "Vroom/Event/CustomEvent/CustomEventCallback.h"

namespace vrm
{

using TriggerCallback = CallbackList<CustomEventCallback>;

} // namespace vrm
