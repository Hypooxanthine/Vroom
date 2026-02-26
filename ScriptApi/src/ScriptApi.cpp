#include "ScriptApi/ScriptApi.h"

#include "ScriptApi/ScriptLibrary.h"

VRM_SCRIPT_API_FUNCTION_IMPL(const char*, VRM_ScriptLibrary_GetLibraryName, (), ())
{
  return "EditorScriptLibrary";
}

VRM_SCRIPT_API_FUNCTION_IMPL(size_t, VRM_ScriptLibrary_GetScriptCount, (), ())
{
  return vrm::ScriptLibrary::Get().getFactories().size();
}

VRM_SCRIPT_API_FUNCTION_IMPL(const char*, VRM_ScriptLibrary_GetScriptName, (size_t index), (index))
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

VRM_SCRIPT_API_FUNCTION_IMPL(vrm::ScriptComponent*, VRM_ScriptLibrary_CreateScript, (size_t index), (index))
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

VRM_SCRIPT_API_FUNCTION_IMPL(void, VRM_ScriptLibrary_DestroyScript, (vrm::ScriptComponent* script), (script))
{
  delete script;
}
