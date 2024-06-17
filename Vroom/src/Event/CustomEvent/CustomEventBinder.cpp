#include "Vroom/Event/CustomEvent/CustomEventBinder.h"

#include "Vroom/Event/CustomEvent/CustomEventManager.h"

namespace vrm
{

CustomEventBinder::CustomEventBinder(CustomEventManager& manager, const std::string& customEventName)
    : m_Manager(manager), m_CustomEventName(customEventName)
{
}

CustomEventBinder::CustomEventBinder(const CustomEventBinder& other)
    : m_Manager(other.m_Manager), m_CustomEventName(other.m_CustomEventName)
{
}

CustomEventBinder::CustomEventBinder(CustomEventBinder&& other)
    : m_Manager(other.m_Manager), m_CustomEventName(std::move(other.m_CustomEventName))
{
}

CustomEventBinder CustomEventBinder::bindInput(Event::Type inputType, CodeType inputCode)
{
    m_Manager.bindInput(m_CustomEventName, inputType, inputCode);
    return *this;
}

CustomEventBinder CustomEventBinder::unbindInput(Event::Type inputType, CodeType inputCode)
{
    m_Manager.unbindInput(m_CustomEventName, inputType, inputCode);
}

CustomEventBinder CustomEventBinder::bindCallback(const CustomEventCallback& cb)
{
    m_Manager.bindCallback(m_CustomEventName, cb);
    return *this;
}

} // namespace vrm
