#include "ScriptApi/ScriptLibrary.h"

using namespace vrm;

ScriptLibrary::ScriptLibrary()
{

}

ScriptLibrary& ScriptLibrary::Get()
{
  static ScriptLibrary instance;

  return instance;
}

void ScriptLibrary::registerFactory(std::unique_ptr<ScriptFactory>&& factory)
{
  m_factories.emplace_back(std::move(factory));
}
