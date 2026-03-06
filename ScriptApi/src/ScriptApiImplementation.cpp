/**
 * This file is the implementation of a script plugin.
 * It is meant to be included by user scripts plugin ONLY, not
 * on the ScriptApi project.
 * Cmake allows this by specifying an interface source for the
 * ScriptApi target : it is not linked with the ScriptApi target,
 * but will be linked with targets depending on it.
 */

#include "ScriptApi/ScriptApi.h"

#include "ScriptApi/ScriptLibrary.h"

VRM_SCRIPT_API const char* VRM_ScriptLibrary_GetLibraryName()
{
  return "EditorScriptLibrary";
}

VRM_SCRIPT_API size_t VRM_ScriptLibrary_GetScriptCount()
{
  return vrm::ScriptLibrary::Get().getFactories().size();
}

VRM_SCRIPT_API const char* VRM_ScriptLibrary_GetScriptName(size_t index)
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

VRM_SCRIPT_API vrm::ScriptComponent* VRM_ScriptLibrary_CreateScript(size_t index)
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

VRM_SCRIPT_API void VRM_ScriptLibrary_DestroyScript(vrm::ScriptComponent* script)
{
  delete script;
}
