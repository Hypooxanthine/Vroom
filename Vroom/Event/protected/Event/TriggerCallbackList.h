#pragma once

#include "Event/CallbackList.h"
#include "Event/TriggerCallback.h"

namespace vrm
{

class TriggerBinder;

using TriggerCallbackList = CallbackList<TriggerCallback, TriggerBinder>;

} // namespace vrm
