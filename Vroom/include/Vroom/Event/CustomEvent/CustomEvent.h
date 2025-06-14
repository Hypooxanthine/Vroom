#pragma once

#include "Vroom/Api.h"
#include "Vroom/Event/CustomEvent/CustomEventCallbackList.h"

namespace vrm
{

class CustomEventBinder;

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
class VRM_API CustomEvent
{
public:
    CustomEvent() = default;

    /**
     * @brief Add a callback to the custom event.
     * 
     * @param cb Callback to add.
     */
    void addCallback(const CustomEventCallback& cb, CustomEventBinder* emitter) { m_Callbacks.addCallback(cb, emitter); }

    /**
     * @brief Trigger the custom event. All associated callbacks are triggered.
     * 
     * @param e Event to pass to the callbacks.
     */
    void trigger(const Event& e) { m_Callbacks.triggerAll(e); }

    void removeCallbacksFromEmitter(CustomEventBinder* emitter) { m_Callbacks.unbindAllFromEmitter(emitter); }

private:
    CustomEventCallbackList m_Callbacks;
};

} // namespace vrm
