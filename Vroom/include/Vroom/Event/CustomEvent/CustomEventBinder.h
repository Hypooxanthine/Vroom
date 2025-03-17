#pragma once

#include <string>

#include "Vroom/Event/Event.h"
#include "Vroom/Event/Codes.h"
#include "Vroom/Event/CustomEvent/CustomEventCallback.h"

namespace vrm
{

class CustomEventManager;

/**
 * @brief Custom event binder class.
 * 
 * This class makes it easier to bind inputs and callbacks to a custom event.
 * 
 */
class CustomEventBinder
{
public:

    /**
     * @brief Construct a new Custom Event Binder object
     * 
     * @param manager CustomEventManager to bind the custom event to.
     * @param customEventName Name of the custom event.
     */
    CustomEventBinder(CustomEventManager& manager, const std::string& customEventName);

    CustomEventBinder(const CustomEventBinder& other) = delete;

    virtual ~CustomEventBinder();

    /**
     * @brief Construct a new Custom Event Binder object
     * 
     * @param other CustomEventBinder to move.
     */
    CustomEventBinder(CustomEventBinder&& other);

    /**
     * @brief Bind an input to the custom event. Since no code is provided, all inputs of the given type will trigger the custom event, regardless of their code.
     * 
     * @param inputType Type of the input.
     * @return CustomEventBinder A binder to the same CustomEvent instance.
     */
    inline CustomEventBinder& bindInput(Event::Type inputType) { return bindInput(inputType, 0); }

    /**
     * @brief Bind an input to the custom event.
     * 
     * @param inputType Type of the input.
     * @param inputCode Code of the input.
     * @return CustomEventBinder A binder to the same CustomEvent instance.
     */
    CustomEventBinder& bindInput(Event::Type inputType, CodeType inputCode);

    /**
     * @brief Bind an input to the custom event.
     * 
     * @tparam C Type of the input code.
     * @param inputType Type of the input.
     * @param inputCode Code of the input.
     * @return CustomEventBinder A binder to the same CustomEvent instance.
     */
    template <typename C>
    CustomEventBinder& bindInput(Event::Type inputType, C inputCode)
    {
        CodeType standardized = static_cast<CodeType>(inputCode);
        return bindInput(inputType, standardized);
    }

    /**
     * @brief Unbind an input from the custom event. Since no code is provided, all inputs of the given type will be unbound.
     * 
     * @param inputType Type of the input.
     * @return CustomEventBinder A binder to the same CustomEvent instance.
     */
    inline CustomEventBinder& unbindInput(Event::Type inputType) { return bindInput(inputType, 0); }

    /**
     * @brief Unbind an input from the custom event.
     * 
     * @param inputType Type of the input.
     * @param inputCode Code of the input.
     * @return CustomEventBinder A binder to the same CustomEvent instance.
     */
    CustomEventBinder& unbindInput(Event::Type inputType, CodeType inputCode);

    /**
     * @brief Unbind an input from the custom event.
     * 
     * @tparam C Type of the input code.
     * @param inputType Type of the input.
     * @param inputCode Code of the input.
     * @return CustomEventBinder A binder to the same CustomEvent instance.
     */
    template <typename C>
    CustomEventBinder& unbindInput(Event::Type inputType, C inputCode)
    {
        CodeType standardized = static_cast<CodeType>(inputCode);
        return unbindInput(inputType, standardized);
    }

    /**
     * @brief Bind a callback to the custom event.
     * 
     * @param cb Callback to bind.
     * @return CustomEventBinder A binder to the same CustomEvent instance.
     */
    CustomEventBinder& bindCallback(const CustomEventCallback& cb);

private:
    CustomEventManager* m_Manager;
    std::string m_CustomEventName;
};

} // namespace vrm
