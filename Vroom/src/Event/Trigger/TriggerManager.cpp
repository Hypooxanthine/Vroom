#include "Vroom/Event/Trigger/TriggerManager.h"

#include "Vroom/Core/Assert.h"

namespace vrm
{

void TriggerManager::check(Event& event)
{
    switch (event.type)
    {
    case Event::Type::KeyPressed:
        event.handled = trigger(event.keyCode, true);
        break;
    case Event::Type::KeyReleased:
        event.handled = trigger(event.keyCode, false);
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

bool TriggerManager::trigger(const KeyCode& key, bool value)
{
    if (!m_Keys.contains(key)) return false;

    for (const auto& triggerName : m_Keys[key])
        m_Triggers[triggerName].trigger(value);
    
    return true;
}

bool TriggerManager::trigger(const MouseCode& mouseButton, bool value)
{
    if (!m_MouseButtons.contains(mouseButton)) return false;

    for (const auto& triggerName : m_MouseButtons[mouseButton])
        m_Triggers[triggerName].trigger(value);

    return true;
}

TriggerBinder TriggerManager::createTrigger(const std::string& name)
{
    VRM_ASSERT(!m_Triggers.contains(name));

    // We create a new event entry : a name and its associated event.
    m_Triggers.insert(std::pair<std::string, Trigger>(name, Trigger()));

    return getBinder(name);
}

TriggerBinder TriggerManager::getBinder(const std::string& name)
{
    return TriggerBinder(*this, name);
}

TriggerBinder TriggerManager::bindInput(const std::string& triggerName, const KeyCode& key)
{
    VRM_DEBUG_ASSERT(!m_Keys[key].contains(triggerName));
    
    m_Keys[key].insert(triggerName);
    return getBinder(triggerName);
}

TriggerBinder TriggerManager::bindInput(const std::string& triggerName, const MouseCode& mouseButton)
{
    VRM_DEBUG_ASSERT(!m_MouseButtons[mouseButton].contains(triggerName));
    
    m_MouseButtons[mouseButton].insert(triggerName);
    return getBinder(triggerName);
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

TriggerBinder TriggerManager::bindCallback(const std::string& triggerName, const TriggerCallback& callback, TriggerBinder* emitter)
{
    VRM_ASSERT(m_Triggers.contains(triggerName));

    m_Triggers[triggerName].bindCallback(callback, emitter);

    return getBinder(triggerName);
}

void TriggerManager::unbindCallbacksFromEmitter(const std::string& triggerName, TriggerBinder* emitter)
{
    VRM_ASSERT(m_Triggers.contains(triggerName));

    m_Triggers.at(triggerName).removeCallbacksFromEmitter(emitter);
}

TriggerManager& TriggerManager::bindPermanentCallback(const std::string& triggerName, const TriggerCallback& cb)
{
    bindCallback(triggerName, cb, nullptr);
    return *this;
}

} // namespace vrm
