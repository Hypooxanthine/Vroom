#pragma once

#include <entt/entt.hpp>

#include "Vroom/Api.h"
#include "Vroom/Scene/Scripting/ScriptEngine.h"
#include "Vroom/Scene/Scripting/ScriptFactory.h"

#include "Vroom/Core/Log.h"

namespace vrm
{

  class Scene;
  class Entity;
  class DeltaTime;

  class VRM_API ScriptComponent
  {
  public:
    friend Entity;
    friend class ScriptEngine;
  public:
    ScriptComponent() = default;
    ScriptComponent(const ScriptComponent&) = delete;
    ScriptComponent& operator=(const ScriptComponent&) = delete;
    ScriptComponent(ScriptComponent&&) = delete;
    ScriptComponent& operator=(ScriptComponent&&) = delete;
    virtual ~ScriptComponent() = default;

    virtual void onSpawn() {}
    virtual void onUpdate(const DeltaTime& dt) {}
    virtual void onDestroy() {}

    inline const std::string& getScriptName() const { return m_scriptName; }

  protected:
    Entity getEntity() const;
    Scene& getScene() const;

  private:
    void setEntityHandle(entt::entity handle);
    void setSceneRef(Scene* scene) { m_Scene = scene; }

  private:
    std::string m_scriptName;
    entt::entity m_EntityHandle = entt::null;
    Scene* m_Scene = nullptr;
  };

#define VRM_FACTORY_CLASS_NAME(ScriptClass) ScriptClass ## _Factory
#define VRM_SCRIPT(ScriptClass) \
  namespace vrm\
  {\
    class VRM_FACTORY_CLASS_NAME(ScriptClass) : public ScriptFactory\
    {\
    public:\
      VRM_FACTORY_CLASS_NAME(ScriptClass)() {}\
      virtual ~VRM_FACTORY_CLASS_NAME(ScriptClass)() {}\
      virtual ScriptClass* create() const override\
      {\
        return new ScriptClass();\
      }\
\
      virtual VRM_FACTORY_CLASS_NAME(ScriptClass)* clone() const override\
      {\
        return new VRM_FACTORY_CLASS_NAME(ScriptClass)();\
      }\
    };\
\
    struct ScriptClass##_Registerer_t\
    {\
      inline ScriptClass##_Registerer_t()\
      {\
        ScriptEngine::Get().registerScript(VRM_GEN_SCRIPT_ID(ScriptClass), std::make_unique<VRM_FACTORY_CLASS_NAME(ScriptClass)>());\
        VRM_LOG_TRACE("Registered {}", VRM_GEN_SCRIPT_ID(ScriptClass));\
      }\
      inline ~ ScriptClass##_Registerer_t()\
      {\
        ScriptEngine::Get().unregisterScript(VRM_GEN_SCRIPT_ID(ScriptClass));\
        VRM_LOG_TRACE("Unregistered {}", VRM_GEN_SCRIPT_ID(ScriptClass));\
      }\
    };\
\
    inline ScriptClass##_Registerer_t VRM_ ## ScriptClass ## _Registerer;\
  \
  }\


} // namespace vrm
