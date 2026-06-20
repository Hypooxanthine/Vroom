#pragma once

#include "ScriptApi/Api.h"
#include <string>

namespace vrm
{

class ScriptComponent;

class VRM_SCRIPTAPI_API ScriptFactory
{
public:

  ScriptFactory(const std::string& name) : m_name(name)
  {}

  virtual ~ScriptFactory()
  {}

  ScriptComponent* create() const;

  const std::string& getName() const
  {
    return m_name;
  }

protected:

  virtual ScriptComponent* delegateCreate() const = 0;

private:

  std::string m_name;
};

} // namespace vrm
