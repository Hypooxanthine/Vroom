#pragma once

#include <list>

#include "Vroom/DataStructure/CallbackList.h"
#include "Vroom/Event/CustomEvent/CustomEventCallback.h"

namespace vrm
{

class CustomEventBinder;

using CustomEventCallbackList = CallbackList<CustomEventCallback, CustomEventBinder>;

} // namespace vrm
