#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "Vroom/Core/Assert.h"
#include "Vroom/Event/CustomEvent/CustomEvent.h"
#include "Vroom/Event/Codes.h"
#include "Vroom/Event/CustomEvent/CustomEventBinder.h"

#define VRM_CUSTOMEVENT_CALLBACK(function) [this] (const vrm::Event& e) { this->function(e); }

namespace vrm
{

/**
 * @brief Custom event manager class.
 * 
 * This class is used to manage custom events. It allows to create, bind and trigger custom events.
 * 
 */
class CustomEventManager
{
public:
    CustomEventManager() = default;

    /**
     * @brief Check if the event triggers any custom event and trigger them.
     * 
     * @param e Event to check.
     */
    void check(Event& e);

    /**
     * @brief Create a new custom event.
     * 
     * @param customEventName Name of the custom event.
     * @return CustomEventBinder Custom event binder to bind inputs and callbacks to the custom event.
     */
    CustomEventBinder createCustomEvent(const std::string& customEventName);

    /**
     * @brief Get the custom event binder for the given custom event.
     * 
     * @param customEventName Name of the custom event.
     * @return CustomEventBinder Custom event binder to bind inputs and callbacks to the custom event.
     */
    CustomEventBinder getBinder(const std::string& customEventName);

    /**
     * @brief Bind a callback to the given custom event.
     * 
     * @param customEventName Name of the custom event.
     * @param cb Callback to bind.
     * @return CustomEventBinder Custom event binder to bind inputs and callbacks to the custom event.
     */
    CustomEventBinder bindCallback(const std::string& customEventName, const CustomEventCallback& cb);

    /**
     * @brief Binds an input to the custom event. Since no code is provided, all inputs of the given type will trigger the custom event, regardless of their code.
     * 
     * @param customEventName Name of the custom event.
     * @param inputType Input type to bind.
     * @return CustomEventBinder Custom event binder to bind inputs and callbacks to the custom event.
     */
    inline CustomEventBinder bindInput(const std::string& customEventName, Event::Type inputType) { return bindInput(customEventName, inputType, 0); }

    /**
     * @brief Binds an input to the custom event.
     * 
     * @tparam C Type of the input code.
     * @param customEventName Name of the custom event.
     * @param inputType Input type to bind.
     * @param inputCode Input code to bind.
     * @return CustomEventBinder Custom event binder to bind inputs and callbacks to the custom event.
     */
    template <typename C>
    CustomEventBinder bindInput(const std::string& customEventName, Event::Type inputType, C inputCode)
    {
        VRM_ASSERT(m_CustomEvents.contains(customEventName));

        CodeType standardizedCode = static_cast<CodeType>(inputCode);
        VRM_DEBUG_ASSERT(!m_RawEventsToCustomEvents[inputType][standardizedCode].contains(customEventName));

        m_RawEventsToCustomEvents[inputType][standardizedCode].insert(customEventName);

        return getBinder(customEventName);
    }

    /**
     * @brief Unbind an input from the custom event. Since no code is provided, all inputs of the given type will be unbound.
     * 
     * @param customEventName Name of the custom event.
     * @param inputType Input type to unbind.
     * @return CustomEventBinder Custom event binder to bind inputs and callbacks to the custom event.
     */
    inline CustomEventBinder unbindInput(const std::string& customEventName, Event::Type inputType) { return unbindInput(customEventName, inputType, 0); }

    /**
     * @brief Unbind an input from the custom event.
     * 
     * @param customEventName Name of the custom event.
     * @param inputType Input type to unbind.
     * @param inputCode Input code to unbind.
     * @return CustomEventBinder Custom event binder to bind inputs and callbacks to the custom event.
     */
    template <typename C>
    CustomEventBinder unbindInput(const std::string& customEventName, Event::Type inputType, C inputCode)
    {
        // Event must be registered.
        VRM_ASSERT(m_RawEventsToCustomEvents.contains(inputType));

        auto& codeToCustomEventName = m_RawEventsToCustomEvents.at(inputType);
        CodeType standardizedCode = static_cast<CodeType>(inputCode);

        // And event code must be registered.
        VRM_ASSERT(codeToCustomEventName.contains(standardizedCode));
        auto& customEventNameList = codeToCustomEventName.at(standardizedCode);

        // Only for debugging. At runtime we don't want to check for something that won't be a problem if it doesn't exist.
        VRM_DEBUG_ASSERT(customEventNameList.contains(customEventName));

        customEventNameList.erase(customEventName);

        return getBinder(customEventName);
    }

private:
    std::unordered_map<std::string, CustomEvent> m_CustomEvents;

    std::unordered_map<Event::Type,
        std::unordered_map<CodeType,
            std::unordered_set<std::string>>> m_RawEventsToCustomEvents;
};

} // namespace vrm
