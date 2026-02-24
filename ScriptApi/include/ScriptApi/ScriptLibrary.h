#pragma once

#include <memory>
#include <vector>

#include <Vroom/Core/Log.h>
#include <Vroom/Scene/Components/ScriptComponent.h>

#include "ScriptApi/Api.h"
#include "ScriptApi/ScriptFactory.h"

namespace vrm
{

class VRM_SCRIPT_API ScriptLibrary
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
