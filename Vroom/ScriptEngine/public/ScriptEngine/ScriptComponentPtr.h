#pragma once

#include <utility>

#include "ScriptEngine/Api.h"
#include "ScriptEngine/ScriptHandle.h"

namespace vrm
{

class ScriptComponent;

class VRM_SCRIPTENGINE_API ScriptComponentPtr
{
public:

  ScriptComponentPtr() = default;

  ScriptComponentPtr(ScriptHandle handle) : m_handle(handle)
  {}

  ~ScriptComponentPtr();

  ScriptComponentPtr(const ScriptComponentPtr&)            = delete;
  ScriptComponentPtr& operator=(const ScriptComponentPtr&) = delete;

  ScriptComponentPtr(ScriptComponentPtr&& other)
  {
    *this = std::move(other);
  }

  ScriptComponentPtr& operator=(ScriptComponentPtr&& other)
  {
    if (this != &other)
    {
      m_handle       = other.m_handle;
      other.m_handle = {};
    }

    return *this;
  }

  ScriptComponent* get() const;

  ScriptComponent& operator*() const
  {
    return *get();
  }

  ScriptComponent* operator->()
  {
    return get();
  }

  bool isValid() const
  {
    return m_handle.isValid();
  }

private:

  ScriptHandle m_handle = {};
};

} // namespace vrm
