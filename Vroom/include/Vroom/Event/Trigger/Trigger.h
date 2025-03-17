#pragma once

#include <cstdint>

#include "Vroom/Event/Trigger/TriggerCallbackList.h"

namespace vrm
{

class TriggerBinder;

/**
 * @page triggers Triggers
 * A trigger is a particular type of event that can be activated and deactivated.
 * For this reason, it only works with inputs that can be pressed (activation) and released (deactivation).
 * A trigger can be bound with multiple inputs and callbacks.
 * For example, a trigger could be "MoveForward". When keys Q or Up arrow are pressed, the trigger is activated.
 * When the trigger is activated, and then Q and Up arrow aren't pressed anymore, the trigger is unactivated.
 * Whenever a trigger is activated or deactivated, all associated callbacks are called with a boolean value 
 * (activated / deactivated).
 */

/**
 * @brief Trigger class.
 * 
 * @see @ref triggers
 */
class Trigger
{
public:
    Trigger() = default;

    /**
     * @brief Add a callback to the trigger.
     * 
     * @param cb Callback to add.
     */
    inline void bindCallback(const TriggerCallback& cb, TriggerBinder* emitter) { m_Callbacks.addCallback(cb, emitter); }

    inline void removeCallbacksFromEmitter(TriggerBinder* emitter) { m_Callbacks.unbindAllFromEmitter(emitter); }

    /**
     * @brief Trigger the trigger. All associated callbacks are triggered.
     * 
     * @param value Value to pass to the callbacks (true if activated, false if deactivated).
     */
    void trigger(bool value);

    /**
     * @brief Check if the trigger is activated.
     * 
     * @return true if the trigger is activated.
     */
    inline bool isTriggered() const { return m_Triggers > 0; }

private:
    // How many times the trigger was activated. If it's 0, the trigger is deactivated.
    uint8_t m_Triggers = 0;

    // Callbacks of the trigger.
    TriggerCallbackList m_Callbacks;
};

} // namespace vrm
