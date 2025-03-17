#pragma once

#include "Vroom/DataStructure/CallbackList.h"
#include "Vroom/Event/Trigger/TriggerCallback.h"

namespace vrm
{

class TriggerBinder;

using TriggerCallbackList = CallbackList<TriggerCallback, TriggerBinder>;

} // namespace vrm
