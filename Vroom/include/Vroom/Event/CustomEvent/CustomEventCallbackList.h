#pragma once

#include <list>

#include "Vroom/DataStructure/CallbackList.h"
#include "Vroom/Event/CustomEvent/CustomEventCallback.h"

namespace vrm
{

using CustomEventCallbackList = CallbackList<CustomEventCallback>;

} // namespace vrm
