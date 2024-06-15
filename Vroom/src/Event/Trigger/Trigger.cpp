#include "Vroom/Event/Trigger/Trigger.h"

namespace vrm
{

void Trigger::trigger(bool value)
{
    if (value)
    {
        m_Triggers++;
        if (m_Triggers == 1) m_Callbacks.triggerAll(true);
    }
    else if (m_Triggers > 0)
    {
        m_Triggers--;
        if (m_Triggers == 0) m_Callbacks.triggerAll(false);
    }
}

} // namespace vrm
