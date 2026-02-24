#pragma once

#include <string>

#include "ScriptApi/Api.h"

namespace vrm
{

class ScriptComponent;

class VRM_SCRIPT_API ScriptFactory
{
public:

  ScriptFactory(const std::string& name) : m_name(name)
  {}

  virtual ~ScriptFactory()
  {}

  virtual ScriptComponent* create() const = 0;

  const std::string& getName() const
  {
    return m_name;
  }

private:

  std::string m_name;
};

} // namespace vrm
