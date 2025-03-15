#pragma once

namespace vrm
{

class ScriptComponent;

class ScriptFactory
{
public:
  virtual [[nodiscard]] ScriptComponent* create() const = 0;

  virtual [[nodiscard]] ScriptFactory* clone() const = 0;

  inline [[nodiscard]] ScriptComponent* operator()() const { return create(); }

};

} // namespace vrm
