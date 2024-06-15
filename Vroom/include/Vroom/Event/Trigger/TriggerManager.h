#pragma once

#include <unordered_map>
#include <unordered_set>
#include <string>

#include "Vroom/Event/Trigger/Trigger.h"
#include "Vroom/Event/Event.h"
#include "Vroom/Event/Trigger/TriggerCallback.h"

#define VRM_TRIGGER_CALLBACK(function) [this] (bool triggered) { this->function(triggered); }

namespace vrm
{

class TriggerManager
{
public:
    class Binder;   
public:
    TriggerManager() = default;

    void check(Event& event);

    void trigger(const KeyCode& key, bool value);
    void trigger(const MouseCode& mouseButton, bool value);

    Binder createTrigger(const std::string& name);

    void bindInput(const std::string& eventName, const KeyCode& key);
    void bindInput(const std::string& eventName, const MouseCode& mouseButton);

    void unbindInput(const std::string& eventName, const KeyCode& key);
    void unbindInput(const std::string& eventName, const MouseCode& mouseButton);

    void bindCallback(const std::string& eventName, const TriggerCallback& callback);

    bool getEventState(const std::string& eventName) const;

public:
    class Binder
    {
    public:
        Binder(TriggerManager& tm, const std::string& eventName) : m_Tm(tm), m_EventName(eventName) {}

        Binder& bindInput(const KeyCode& key)
        {
            m_Tm.bindInput(m_EventName, key);
            return *this;
        }

        Binder& bindInput(const MouseCode& mouse)
        {
            m_Tm.bindInput(m_EventName, mouse);
            return *this;
        }

        Binder& bindCallback(const TriggerCallback& callback)
        {
            m_Tm.bindCallback(m_EventName, callback);
            return *this;
        }

    private:
        TriggerManager& m_Tm;
        std::string m_EventName;
    };

private:
    std::unordered_map<std::string, Trigger> m_Triggers;

    std::unordered_map<KeyCode, std::unordered_set<std::string>> m_Keys;
    std::unordered_map<MouseCode, std::unordered_set<std::string>> m_MouseButtons;

    std::unordered_map<KeyCode, bool> m_KeyStates;
    std::unordered_map<MouseCode, bool> m_MouseStates;
};

} // namespace vrm
