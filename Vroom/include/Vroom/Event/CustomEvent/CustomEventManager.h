#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "Vroom/Core/Assert.h"
#include "Vroom/Event/CustomEvent/CustomEvent.h"
#include "Vroom/Event/Codes.h"
#include "Vroom/Event/CustomEvent/CustomEventBinder.h"

namespace vrm
{

class CustomEventManager
{
public:
    CustomEventManager() = default;

    void check(const Event& e);

    CustomEventBinder createCustomEvent(const std::string& customEventName);

    CustomEventBinder bindCallback(const std::string& customEventName, const CustomEventCallback& cb);

    inline CustomEventBinder bindInput(const std::string& customEventName, Event::Type inputType) { return bindInput(customEventName, inputType, 0); }

    template <typename C>
    CustomEventBinder bindInput(const std::string& customEventName, Event::Type inputType, C inputCode)
    {
        VRM_ASSERT(m_CustomEvents.contains(customEventName));

        CodeType standardizedCode = static_cast<CodeType>(inputCode);
        VRM_DEBUG_ASSERT(!m_RawEventsToCustomEvents[inputType][standardizedCode].contains(customEventName));

        m_RawEventsToCustomEvents[inputType][standardizedCode].insert(customEventName);

        return CustomEventBinder(*this, customEventName);
    }

    inline CustomEventBinder unbindInput(const std::string& customEventName, Event::Type inputType) { return unbindInput(customEventName, inputType, 0); }

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

        return CustomEventBinder(*this, customEventName);
    }

private:
    std::unordered_map<std::string, CustomEvent> m_CustomEvents;

    std::unordered_map<Event::Type,
        std::unordered_map<CodeType,
            std::unordered_set<std::string>>> m_RawEventsToCustomEvents;
};

} // namespace vrm
