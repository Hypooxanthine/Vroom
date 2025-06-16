#pragma once

#include "Vroom/Api.h"

namespace vrm
{

class ScriptComponent;

class VRM_API ScriptFactory
{
public:
  ScriptFactory() {}
  virtual ~ScriptFactory() {}

  virtual ScriptComponent* create() const = 0;

  virtual ScriptFactory* clone() const = 0;

  inline ScriptComponent* operator()() const { return create(); }

};

} // namespace vrm
