#pragma once

#include "ScriptApi/Api.h"
#include "ScriptApi/ScriptApi.h"

#define VRM_FORCE_LINKER_TO_INCLUDE_SYMBOL(symbol) \
  VRM_SCRIPT_API_C inline void* VRM_FORCE_##symbol()       \
  {                                                \
    return (void*)&symbol;                                \
  }

VRM_FORCE_LINKER_TO_INCLUDE_SYMBOL(VRM_ScriptLibrary_GetLibraryName)
VRM_FORCE_LINKER_TO_INCLUDE_SYMBOL(VRM_ScriptLibrary_GetScriptCount)
VRM_FORCE_LINKER_TO_INCLUDE_SYMBOL(VRM_ScriptLibrary_GetScriptName)
VRM_FORCE_LINKER_TO_INCLUDE_SYMBOL(VRM_ScriptLibrary_CreateScript)
VRM_FORCE_LINKER_TO_INCLUDE_SYMBOL(VRM_ScriptLibrary_DestroyScript)
