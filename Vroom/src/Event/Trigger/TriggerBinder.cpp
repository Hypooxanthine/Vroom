#include "Vroom/Event/Trigger/TriggerBinder.h"

#include "Vroom/Event/Trigger/TriggerManager.h"

namespace vrm
{

TriggerBinder::TriggerBinder(TriggerBinder&& other)
    : m_Tm(other.m_Tm), m_TriggerName(std::move(other.m_TriggerName))
{
    
}

TriggerBinder::TriggerBinder(const TriggerBinder& other)
    : m_Tm(other.m_Tm), m_TriggerName(other.m_TriggerName)
{
    
}

TriggerBinder TriggerBinder::bindInput(const KeyCode& key)
{
    m_Tm.bindInput(m_TriggerName, key);
    return *this;
}

TriggerBinder TriggerBinder::bindInput(const MouseCode& mouse)
{
    m_Tm.bindInput(m_TriggerName, mouse);
    return *this;
}

TriggerBinder TriggerBinder::bindCallback(const TriggerCallback& callback)
{
    m_Tm.bindCallback(m_TriggerName, callback);
    return *this;
}

} // namespace vrm
