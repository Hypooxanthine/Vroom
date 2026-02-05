#pragma once

#include <string>

namespace vrm
{

class ScriptComponent;

class ScriptFactory
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
