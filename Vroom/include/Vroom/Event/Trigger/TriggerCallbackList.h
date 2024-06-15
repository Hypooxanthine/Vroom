#pragma once

#include <list>

#include "Vroom/Event/Trigger/TriggerCallback.h"

namespace vrm
{

class TriggerCallbackList
{
public:
    TriggerCallbackList() = default;

    inline void addCallback(const TriggerCallback& tc) { m_List.push_back(tc); }
    void triggerAll(bool value);

private:
    std::list<TriggerCallback> m_List;
};

} // namespace vrm
