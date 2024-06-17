#pragma once

#include <string>

#include "Vroom/Event/Event.h"
#include "Vroom/Event/Codes.h"
#include "Vroom/Event/CustomEvent/CustomEventCallback.h"

namespace vrm
{

class CustomEventManager;

class CustomEventBinder
{
public:
    CustomEventBinder(CustomEventManager& manager, const std::string& customEventName);
    CustomEventBinder(const CustomEventBinder& other);
    CustomEventBinder(CustomEventBinder&& other);

    inline CustomEventBinder bindInput(Event::Type inputType) { return bindInput(inputType, 0); }
    CustomEventBinder bindInput(Event::Type inputType, CodeType inputCode);

    template <typename C>
    CustomEventBinder bindInput(Event::Type inputType, C inputCode)
    {
        CodeType standardized = static_cast<CodeType>(inputCode);
        return bindInput(inputType, standardized);
    }

    inline CustomEventBinder unbindInput(Event::Type inputType) { return bindInput(inputType, 0); }
    CustomEventBinder unbindInput(Event::Type inputType, CodeType inputCode);

    template <typename C>
    CustomEventBinder unbindInput(Event::Type inputType, C inputCode)
    {
        CodeType standardized = static_cast<CodeType>(inputCode);
        return unbindInput(inputType, standardized);
    }

    CustomEventBinder bindCallback(const CustomEventCallback& cb);

private:
    CustomEventManager& m_Manager;
    std::string m_CustomEventName;
};

} // namespace vrm
