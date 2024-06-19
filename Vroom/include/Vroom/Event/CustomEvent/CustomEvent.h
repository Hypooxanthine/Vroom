#pragma once

#include "Vroom/Event/CustomEvent/CustomEventCallbackList.h"

namespace vrm
{

/**
 * @page custom_events Custom events
 * A custom event is a particular type of event that will be triggered when specific inputs are received.
 * When a custom event is triggered, all associated callbacks are triggered as well, given the raw event object
 * as a parameter.
 * A custom event can be bound with multiple inputs and callbacks.
 * Unlike triggers, custom events don't have on/off state. Thus, there is no limitation on event types, and you can
 * use any event type (they don't have to have a pressed/released state), such as vrm::Event::Type::Exit,
 * vrm::Event::Type::Scroll, ...
 */

/**
 * @brief Custom event class.
 * 
 * @see @ref custom_events
 * 
 */
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
