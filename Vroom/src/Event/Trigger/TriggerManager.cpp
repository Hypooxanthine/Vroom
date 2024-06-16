#include "Vroom/Event/Trigger/TriggerManager.h"

#include "Vroom/Core/Assert.h"

namespace vrm
{

void TriggerManager::check(Event& event)
{
    switch (event.type)
    {
    case Event::Type::KeyPressed:
        trigger(event.keyCode, true);
        break;
    case Event::Type::KeyReleased:
        trigger(event.keyCode, false);
        break;
    case Event::Type::MousePressed:
        trigger(event.mouseCode, true);
        break;
    case Event::Type::MouseReleased:
        trigger(event.mouseCode, false);
        break;
    default:
        break;
    }
}

void TriggerManager::trigger(const KeyCode& key, bool value)
{
    if (!m_Keys.contains(key)) return;

    for (const auto& triggerName : m_Keys[key])
        m_Triggers[triggerName].trigger(value);
}

void TriggerManager::trigger(const MouseCode& mouseButton, bool value)
{
    if (!m_MouseButtons.contains(mouseButton)) return;

    for (const auto& triggerName : m_MouseButtons[mouseButton])
        m_Triggers[triggerName].trigger(value);
}

TriggerBinder TriggerManager::createTrigger(const std::string& name)
{
    VRM_ASSERT(!m_Triggers.contains(name));

    // We create a new event entry : a name and its associated event.
    m_Triggers.insert(std::pair<std::string, Trigger>(name, Trigger()));

    return TriggerBinder(*this, name);
}

TriggerBinder TriggerManager::bindInput(const std::string& triggerName, const KeyCode& key)
{
    m_Keys[key].insert(triggerName);
    return TriggerBinder(*this, triggerName);
}

TriggerBinder TriggerManager::bindInput(const std::string& triggerName, const MouseCode& mouseButton)
{
    m_MouseButtons[mouseButton].insert(triggerName);

    return TriggerBinder(*this, triggerName);
}

void TriggerManager::unbindInput(const std::string& triggerName, const KeyCode& key)
{
    if (!m_Keys.contains(key) || !m_Keys[key].contains(triggerName)) return;

    m_Keys[key].erase(triggerName);
}

void TriggerManager::unbindInput(const std::string& triggerName, const MouseCode& mouseButton)
{
    if (!m_MouseButtons.contains(mouseButton) || !m_MouseButtons[mouseButton].contains(triggerName)) return;

    m_MouseButtons[mouseButton].erase(triggerName);
}

TriggerBinder TriggerManager::bindCallback(const std::string& triggerName, const TriggerCallback& callback)
{
    VRM_ASSERT(m_Triggers.contains(triggerName));

    m_Triggers[triggerName].bindCallback(callback);

    return TriggerBinder(*this, triggerName);
}

} // namespace vrm
