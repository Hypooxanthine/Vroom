#pragma once

#include <Vroom/Scene/Components/ScriptComponent.h>

#include "ScriptApi/Api.h"

extern "C" {

VRM_SCRIPT_API_C const char* VRM_ScriptLibrary_GetLibraryName();

VRM_SCRIPT_API_C size_t      VRM_ScriptLibrary_GetScriptCount();
VRM_SCRIPT_API_C const char* VRM_ScriptLibrary_GetScriptName(size_t index);

VRM_SCRIPT_API_C vrm::ScriptComponent* VRM_ScriptLibrary_CreateScript(size_t index);
VRM_SCRIPT_API_C void                  VRM_ScriptLibrary_DestroyScript(vrm::ScriptComponent* script);

} // extern "C"
