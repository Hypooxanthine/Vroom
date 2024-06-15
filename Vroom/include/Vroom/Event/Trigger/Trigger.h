#pragma once

#include "Vroom/Event/Trigger/TriggerCallbackList.h"

namespace vrm
{

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
