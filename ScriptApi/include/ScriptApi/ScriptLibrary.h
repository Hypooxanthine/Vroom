#pragma once

#include <memory>
#include <vector>

#include <Vroom/Core/Log.h>
#include <Vroom/Scene/Components/ScriptComponent.h>

#include "ScriptApi/ScriptFactory.h"

namespace vrm
{

class ScriptLibrary
{
public:

  ScriptLibrary(const ScriptLibrary&)            = delete;
  ScriptLibrary& operator=(const ScriptLibrary&) = delete;

  ScriptLibrary(ScriptLibrary&&)            = delete;
  ScriptLibrary& operator=(ScriptLibrary&&) = delete;

  static ScriptLibrary& Get();

  void registerFactory(std::unique_ptr<ScriptFactory>&& factory);

  inline const std::vector<std::unique_ptr<ScriptFactory>>& getFactories() const
  {
    return m_factories;
  }

private:

  ScriptLibrary();

private:

  std::vector<std::unique_ptr<ScriptFactory>> m_factories;
};

} // namespace vrm
