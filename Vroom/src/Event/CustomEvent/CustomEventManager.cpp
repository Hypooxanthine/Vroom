#include "Vroom/Event/CustomEvent/CustomEventManager.h"

namespace vrm
{

void CustomEventManager::check(const Event& e)
{
    // If event type is registered
    if (!m_RawEventsToCustomEvents.contains(e.type)) return;

    auto& codeToCustomEventName = m_RawEventsToCustomEvents.at(e.type);

    // We need to call events bound to no code, i.e. CodeType = 0, if exists
    if (codeToCustomEventName.contains(0))
    {
        for (const auto& customEventName : codeToCustomEventName.at(0))
        {
            // We call the custom event
            m_CustomEvents.at(customEventName).trigger(e);
        }
    }

    // We already checked if zero was stored. So we don't want to call callbacks twice. We return.
    if (e.standardizedCode == 0)
        return;

    // And event code is registered
    if (codeToCustomEventName.contains(e.standardizedCode))
    {
        // For each custom event associated with the pair {event type, event code}
        for (const auto& customEventName : codeToCustomEventName.at(e.standardizedCode))
        {
            // We call the custom event
            m_CustomEvents.at(customEventName).trigger(e);
        }
    }


}

CustomEventBinder CustomEventManager::createCustomEvent(const std::string& customEventName)
{
    VRM_ASSERT(!m_CustomEvents.contains(customEventName));

    m_CustomEvents.emplace(customEventName, CustomEvent());

    return CustomEventBinder(*this, customEventName);
}

CustomEventBinder CustomEventManager::bindCallback(const std::string& customEventName, const CustomEventCallback& cb)
{
    VRM_ASSERT(m_CustomEvents.contains(customEventName));

    m_CustomEvents.at(customEventName).addCallback(cb);

    return CustomEventBinder(*this, customEventName);
}

} // namespace vrm
