#pragma once

#include <cstdint>

#include "Vroom/Event/Trigger/TriggerCallbackList.h"

namespace vrm
{

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

    inline void bindCallback(const TriggerCallback& cb) { m_Callbacks.addCallback(cb); }

    void trigger(bool value);

    inline bool isTriggered() const { return m_Triggers > 0; }

private:
    // The number of triggers currently used
    uint8_t m_Triggers = 0;

    // Callbacks of the trigger.
    TriggerCallbackList m_Callbacks;
};

} // namespace vrm
