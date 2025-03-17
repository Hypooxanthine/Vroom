#include "Vroom/Event/CustomEvent/CustomEventBinder.h"

#include "Vroom/Event/CustomEvent/CustomEventManager.h"

using namespace vrm;

CustomEventBinder::CustomEventBinder(CustomEventManager &manager, const std::string &customEventName)
    : m_Manager(&manager), m_CustomEventName(customEventName)
{
}

CustomEventBinder::CustomEventBinder(CustomEventBinder &&other)
    : m_Manager(other.m_Manager), m_CustomEventName(std::move(other.m_CustomEventName))
{
  other.m_Manager = nullptr;
}

CustomEventBinder::~CustomEventBinder()
{
  m_Manager->unbindCallbacksFromEmitter(m_CustomEventName, this);
}

CustomEventBinder &CustomEventBinder::bindInput(Event::Type inputType, CodeType inputCode)
{
  m_Manager->bindInput(m_CustomEventName, inputType, inputCode);
  return *this;
}

CustomEventBinder &CustomEventBinder::unbindInput(Event::Type inputType, CodeType inputCode)
{
  m_Manager->unbindInput(m_CustomEventName, inputType, inputCode);
  return *this;
}

CustomEventBinder &CustomEventBinder::bindCallback(const CustomEventCallback &cb)
{
  m_Manager->bindCallback(m_CustomEventName, cb, this);
  return *this;
}
