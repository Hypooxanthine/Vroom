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

/**
 * @brief Trigger manager class.
 * 
 * This class is used to manage triggers. It allows to create, bind and trigger triggers.
 * 
 */
class TriggerManager
{
public:
    TriggerManager() = default;

    /**
     * @brief Check if the event triggers any Trigger and trigger them.
     * 
     * @param event Event to check.
     */
    void check(Event& event);

    /**
     * @brief Trigger any Trigger associated with the given key.
     * 
     * @param key Key to check.
     * @param value Value of the key (true if pressed, false if released).
     * @return true if the key triggers any Trigger.
     * @return false if the key doesn't trigger any Trigger.
     */
    bool trigger(const KeyCode& key, bool value);

    /**
     * @brief Trigger any Trigger associated with the given mouse button.
     * 
     * @param mouseButton Mouse button to check.
     * @param value Value of the mouse button (true if pressed, false if released).
     * @return true if the mouse button triggers any Trigger.
     * @return false if the mouse button doesn't trigger any Trigger.
     */
    bool trigger(const MouseCode& mouseButton, bool value);

    /**
     * @brief Create a new Trigger.
     * 
     * @param name Name of the Trigger.
     * @return TriggerBinder Trigger binder to bind inputs and callbacks to the Trigger.
     */
    TriggerBinder createTrigger(const std::string& name);

    /**
     * @brief Get the Trigger binder for the given Trigger.
     * 
     * @param name Name of the Trigger.
     * @return TriggerBinder Trigger binder to bind inputs and callbacks to the Trigger.
     */
    TriggerBinder getBinder(const std::string& name);

    /**
     * @brief Bind a key input to the given Trigger.
     * 
     * @param triggerName Name of the Trigger.
     * @param key Key to bind.
     * @return TriggerBinder Trigger binder to bind inputs and callbacks to the Trigger.
     */
    TriggerBinder bindInput(const std::string& triggerName, const KeyCode& key);

    /**
     * @brief Bind a mouse input to the given Trigger.
     * 
     * @param triggerName Name of the Trigger.
     * @param mouseButton Mouse button to bind.
     * @return TriggerBinder Trigger binder to bind inputs and callbacks to the Trigger.
     */
    TriggerBinder bindInput(const std::string& triggerName, const MouseCode& mouseButton);

    /**
     * @brief Unbind a key input from the given Trigger.
     * 
     * @param triggerName Name of the Trigger.
     * @param key Key to unbind.
     */
    void unbindInput(const std::string& triggerName, const KeyCode& key);

    /**
     * @brief Unbind a mouse input from the given Trigger.
     * 
     * @param triggerName Name of the Trigger.
     * @param mouseButton Mouse button to unbind.
     */
    void unbindInput(const std::string& triggerName, const MouseCode& mouseButton);

    /**
     * @brief Bind a callback to the given Trigger.
     * 
     * @param triggerName Name of the Trigger.
     * @param callback Callback to bind.
     * @return TriggerBinder Trigger binder to bind inputs and callbacks to the Trigger.
     */
    TriggerBinder bindCallback(const std::string& triggerName, const TriggerCallback& callback);

private:
    std::unordered_map<std::string, Trigger> m_Triggers;

    std::unordered_map<KeyCode, std::unordered_set<std::string>> m_Keys;
    std::unordered_map<MouseCode, std::unordered_set<std::string>> m_MouseButtons;
};

} // namespace vrm
