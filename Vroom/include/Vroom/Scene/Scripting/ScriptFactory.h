#pragma once

namespace vrm
{

class ScriptComponent;

class ScriptFactory
{
public:
  virtual ScriptComponent* create() const = 0;

  virtual ScriptFactory* clone() const = 0;

  inline ScriptComponent* operator()() const { return create(); }

};

} // namespace vrm
