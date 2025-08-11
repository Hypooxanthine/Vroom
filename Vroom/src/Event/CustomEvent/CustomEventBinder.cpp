#include "Vroom/Event/CustomEvent/CustomEventBinder.h"

#include "Vroom/Event/CustomEvent/CustomEventManager.h"

using namespace vrm;

CustomEventBinder::CustomEventBinder(CustomEventManager &manager, const std::string &customEventName)
    : m_Manager(&manager), m_CustomEventName(customEventName)
{
}

CustomEventBinder::CustomEventBinder(CustomEventBinder &&other)
{
  *this = std::move(other);
}

CustomEventBinder& CustomEventBinder::operator=(CustomEventBinder&& other)
{
  if (this != &other)
  {
    m_Manager = other.m_Manager;
    m_CustomEventName = std::move(other.m_CustomEventName);

    other.m_Manager = nullptr;
  }

  return *this;
}

CustomEventBinder::~CustomEventBinder()
{
  if (m_Manager)
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
