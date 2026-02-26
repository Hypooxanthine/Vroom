#pragma once

#include <Vroom/Scene/Components/ScriptComponent.h>

#include "ScriptApi/Api.h"
#include "ScriptApi/ScriptLibrary.h"

#ifdef VRM_TARGET_SCRIPT_API
#define VRM_SCRIPT_API_FUNCTION_C_DECL(ReturnType, Name, ArgsDecl)
#else
#define VRM_SCRIPT_API_FUNCTION_C_DECL(ReturnType, Name, ArgsDecl) \
  extern "C" {                                                     \
  VRM_EXPORT_SYMBOLS ReturnType Name ArgsDecl;                     \
  }
#endif

#ifdef VRM_TARGET_SCRIPT_API
#define VRM_SCRIPT_API_FUNCTION_C_IMPL(ReturnType, Name, ArgsDecl, ArgsCall)
#else
#define VRM_SCRIPT_API_FUNCTION_C_IMPL(ReturnType, Name, ArgsDecl, ArgsCall) \
  inline ReturnType Name ArgsDecl                                            \
  {                                                                          \
    return StaticLib_##Name ArgsCall;                                        \
  }
#endif

#define VRM_SCRIPT_API_FUNCTION_IMPL(ReturnType, Name, ArgsDecl, ArgsCall) ReturnType StaticLib_##Name ArgsDecl

#define VRM_SCRIPT_API_FUNCTION(ReturnType, Name, ArgsDecl, ArgsCall)  \
  ReturnType StaticLib_##Name ArgsDecl;                                \
  VRM_SCRIPT_API_FUNCTION_C_DECL(ReturnType, Name, ArgsDecl)           \
  VRM_SCRIPT_API_FUNCTION_C_IMPL(ReturnType, Name, ArgsDecl, ArgsCall)

VRM_SCRIPT_API_FUNCTION(const char*, VRM_ScriptLibrary_GetLibraryName, (), ())
VRM_SCRIPT_API_FUNCTION(size_t, VRM_ScriptLibrary_GetScriptCount, (), ())
VRM_SCRIPT_API_FUNCTION(const char*, VRM_ScriptLibrary_GetScriptName, (size_t index), (index))
VRM_SCRIPT_API_FUNCTION(vrm::ScriptComponent*, VRM_ScriptLibrary_CreateScript, (size_t index), (index))
VRM_SCRIPT_API_FUNCTION(void, VRM_ScriptLibrary_DestroyScript, (vrm::ScriptComponent * script), (script))

#define VRM_STRINGIFY(str)                  #str
#define VRM_GEN_SCRIPT_ID(ScriptClass)      VRM_STRINGIFY(VRM_##ScriptClass##_ID)
#define VRM_FACTORY_CLASS_NAME(ScriptClass) ScriptClass##_Factory
#define VRM_SCRIPT(ScriptClass)                                                                           \
  namespace vrm                                                                                           \
  {                                                                                                       \
                                                                                                          \
  template <>                                                                                             \
  struct ScriptComponentTraits<ScriptClass>                                                               \
  {                                                                                                       \
    inline static constexpr std::string_view scriptId = VRM_GEN_SCRIPT_ID(ScriptClass);                   \
  };                                                                                                      \
                                                                                                          \
  class VRM_FACTORY_CLASS_NAME(ScriptClass) : public ScriptFactory                                        \
  {                                                                                                       \
  public:                                                                                                 \
                                                                                                          \
    VRM_FACTORY_CLASS_NAME(ScriptClass)() : ScriptFactory(VRM_GEN_SCRIPT_ID(ScriptClass))                 \
    {}                                                                                                    \
    virtual ~VRM_FACTORY_CLASS_NAME(ScriptClass)()                                                        \
    {}                                                                                                    \
    virtual ScriptClass* create() const override                                                          \
    {                                                                                                     \
      return new ScriptClass();                                                                           \
    }                                                                                                     \
  };                                                                                                      \
                                                                                                          \
  struct ScriptClass##_Registerer_t                                                                       \
  {                                                                                                       \
    inline ScriptClass##_Registerer_t()                                                                   \
    {                                                                                                     \
      vrm::ScriptLibrary::Get().registerFactory(std::make_unique<VRM_FACTORY_CLASS_NAME(ScriptClass)>()); \
      VRM_LOG_TRACE("Registered {}", VRM_GEN_SCRIPT_ID(ScriptClass));                                     \
    }                                                                                                     \
  };                                                                                                      \
                                                                                                          \
  inline ScriptClass##_Registerer_t VRM_##ScriptClass##_Registerer;                                       \
  }
