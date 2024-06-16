#pragma once

#include <string>

#include "Vroom/Event/Codes.h"
#include "Vroom/Event/Trigger/TriggerCallback.h"
#include "Vroom/Event/Event.h"

namespace vrm
{

class TriggerManager;

class TriggerBinder
{
public:
    TriggerBinder(TriggerManager& tm, const std::string& triggerName) : m_Tm(tm), m_TriggerName(triggerName) {}
    TriggerBinder(const TriggerBinder& other);
    TriggerBinder(TriggerBinder&& other);

    TriggerBinder bindInput(const KeyCode& key);
    TriggerBinder bindInput(const MouseCode& mouse);
    TriggerBinder bindCallback(const TriggerCallback& callback);

private:
    TriggerManager& m_Tm;
    std::string m_TriggerName;
};

}