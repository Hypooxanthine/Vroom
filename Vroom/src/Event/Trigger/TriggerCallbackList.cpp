#include "Vroom/Event/Trigger/TriggerCallbackList.h"

namespace vrm
{

void TriggerCallbackList::triggerAll(bool value)
{
    for (auto& cb : m_List)
        cb(value);
}   

} // namespace vrm

