#pragma once

#include "Event/CallbackList.h"
#include "Event/CustomEventCallback.h"

namespace vrm
{

class CustomEventBinder;

using CustomEventCallbackList = CallbackList<CustomEventCallback, CustomEventBinder>;

} // namespace vrm
