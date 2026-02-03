#include "ScriptApi/ScriptApi.h"

#include "ScriptApi/ScriptLibrary.h"

const char* VRM_ScriptLibrary_GetLibraryName()
{
  return "EditorScriptLibrary";
}

size_t VRM_ScriptLibrary_GetScriptCount()
{
  return vrm::ScriptLibrary::Get().getFactories().size();
}

const char* VRM_ScriptLibrary_GetScriptName(size_t index)
{
  const auto& factories = vrm::ScriptLibrary::Get().getFactories();

  if (index < factories.size())
  {
    return factories.at(index)->getName().c_str();
  }
  else
  {
    return "";
  }
}

vrm::ScriptComponent* VRM_ScriptLibrary_CreateScript(size_t index)
{
  const auto& factories = vrm::ScriptLibrary::Get().getFactories();

  if (index < factories.size())
  {
    return factories.at(index)->create();
  }
  else
  {
    return nullptr;
  }
}

void VRM_ScriptLibrary_DestroyScript(vrm::ScriptComponent* script)
{
  delete script;
}
