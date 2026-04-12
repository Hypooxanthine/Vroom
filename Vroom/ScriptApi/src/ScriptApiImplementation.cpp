/**
 * This file is the implementation of a script plugin.
 * It is meant to be included by user scripts plugin ONLY, not
 * on the ScriptApi project.
 */

#if defined(VRM_PLATFORM_LINUX)
  #define VRM_EXPORT_SYMBOLS __attribute__((visibility ("default")))
#elif defined(VRM_PLATFORM_WINDOWS)
  #define VRM_EXPORT_SYMBOLS __declspec(dllexport)
#else
  static_assert(false, "Unsupported platform");
#endif

#define VRM_SCRIPTAPI_API extern "C" VRM_EXPORT_SYMBOLS

#include "ScriptApi/ScriptLibrary.h"

VRM_SCRIPTAPI_API const char* VRM_ScriptLibrary_GetLibraryName()
{
  return "EditorScriptLibrary";
}

VRM_SCRIPTAPI_API size_t VRM_ScriptLibrary_GetScriptCount()
{
  return vrm::ScriptLibrary::Get().getFactories().size();
}

VRM_SCRIPTAPI_API const char* VRM_ScriptLibrary_GetScriptName(size_t index)
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

VRM_SCRIPTAPI_API vrm::ScriptComponent* VRM_ScriptLibrary_CreateScript(size_t index)
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

VRM_SCRIPTAPI_API void VRM_ScriptLibrary_DestroyScript(vrm::ScriptComponent* script)
{
  delete script;
}
