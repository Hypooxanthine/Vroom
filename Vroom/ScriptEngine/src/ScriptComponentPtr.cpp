#include "ScriptEngine/ScriptComponentPtr.h"

#include "ScriptEngine/ScriptEngine.h"

using namespace vrm;

ScriptComponentPtr::~ScriptComponentPtr()
{
  if (m_handle.isValid())
  {
    ScriptEngine::Get()._destroyScript(m_handle);
    m_handle = {};
  }
}

ScriptComponent* ScriptComponentPtr::get() const
{
  return ScriptEngine::Get()._getScript(m_handle);
}
