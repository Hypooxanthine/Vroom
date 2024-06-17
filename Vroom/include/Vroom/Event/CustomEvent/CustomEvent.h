#pragma once

#include "Vroom/Event/CustomEvent/CustomEventCallbackList.h"

namespace vrm
{

class CustomEvent
{
public:
    CustomEvent() = default;

    void addCallback(const CustomEventCallback& cb) { m_Callbacks.addCallback(cb); }
    void trigger(const Event& e) { m_Callbacks.triggerAll(e); }

private:
    CustomEventCallbackList m_Callbacks;
};

} // namespace vrm
