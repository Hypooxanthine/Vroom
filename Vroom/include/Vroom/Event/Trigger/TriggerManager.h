#pragma once

#include <unordered_map>
#include <unordered_set>
#include <string>

#include "Vroom/Core/Assert.h"
#include "Vroom/Event/Trigger/Trigger.h"
#include "Vroom/Event/Event.h"
#include "Vroom/Event/Trigger/TriggerCallback.h"
#include "Vroom/Event/Trigger/TriggerBinder.h"

#define VRM_TRIGGER_CALLBACK(function) [this] (bool triggered) { this->function(triggered); }

namespace vrm
{

class TriggerManager
{
public:
    TriggerManager() = default;

    void check(Event& event);

    void trigger(const KeyCode& key, bool value);
    void trigger(const MouseCode& mouseButton, bool value);

    TriggerBinder createTrigger(const std::string& name);

    TriggerBinder bindInput(const std::string& triggerName, const KeyCode& key);
    TriggerBinder bindInput(const std::string& triggerName, const MouseCode& mouseButton);

    void unbindInput(const std::string& triggerName, const KeyCode& key);
    void unbindInput(const std::string& triggerName, const MouseCode& mouseButton);

    TriggerBinder bindCallback(const std::string& triggerName, const TriggerCallback& callback);

private:
    std::unordered_map<std::string, Trigger> m_Triggers;

    std::unordered_map<KeyCode, std::unordered_set<std::string>> m_Keys;
    std::unordered_map<MouseCode, std::unordered_set<std::string>> m_MouseButtons;
};

} // namespace vrm
