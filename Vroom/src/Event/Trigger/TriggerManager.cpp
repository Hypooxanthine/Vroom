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

    // We check if the key is already pressed or already released. This can happen when the window focus is lost and gained.
    if (value && m_KeyStates[key]) return;
    if (!value && !m_KeyStates[key]) return;

    m_KeyStates[key] = value;

    for (const auto& eventName : m_Keys[key])
        m_Triggers[eventName].trigger(value);
}

void TriggerManager::trigger(const MouseCode& mouseButton, bool value)
{
    if (!m_MouseButtons.contains(mouseButton)) return;

    // We check if the button is already pressed or already released. This can happen when the window focus is lost and gained.
    if (value && m_MouseStates[mouseButton]) return;
    if (!value && !m_MouseStates[mouseButton]) return;

    m_MouseStates[mouseButton] = value;

    for (const auto& eventName : m_MouseButtons[mouseButton])
        m_Triggers[eventName].trigger(value);
}

TriggerManager::Binder TriggerManager::createTrigger(const std::string& name)
{
    VRM_ASSERT(!m_Triggers.contains(name));

    // We create a new event entry : a name and its associated event.
    m_Triggers.insert(std::pair<std::string, Trigger>(name, Trigger()));

    return Binder(*this, name);
}

void TriggerManager::bindInput(const std::string& eventName, const KeyCode& key)
{
    if (!m_Keys.contains(key))
    {
        m_Keys[key] = std::unordered_set<std::string>{ eventName };
        m_KeyStates[key] = false;
    }
    
    if (m_Keys[key].contains(eventName)) return;

    m_Keys[key].insert(eventName);
}

void TriggerManager::bindInput(const std::string& eventName, const MouseCode& mouseButton)
{
    if (!m_MouseButtons.contains(mouseButton))
    {
        m_MouseButtons[mouseButton] = std::unordered_set<std::string>{ eventName };
        m_MouseStates[mouseButton] = false;
    }

    if (m_MouseButtons[mouseButton].contains(eventName)) return;

    m_MouseButtons[mouseButton].insert(eventName);
}

void TriggerManager::unbindInput(const std::string& eventName, const KeyCode& key)
{
    if (!m_Keys.contains(key) || !m_Keys[key].contains(eventName)) return;

    m_Keys[key].erase(eventName);
}

void TriggerManager::unbindInput(const std::string& eventName, const MouseCode& mouseButton)
{
    if (!m_MouseButtons.contains(mouseButton) || !m_MouseButtons[mouseButton].contains(eventName)) return;

    m_MouseButtons[mouseButton].erase(eventName);
}

void TriggerManager::bindCallback(const std::string& eventName, const TriggerCallback& callback)
{
    VRM_ASSERT(m_Triggers.contains(eventName));

    m_Triggers[eventName].bindCallback(callback);
}

bool TriggerManager::getEventState(const std::string& eventName) const
{
    VRM_ASSERT(m_Triggers.contains(eventName));

    return m_Triggers.at(eventName).isTriggered();
}

} // namespace vrm
