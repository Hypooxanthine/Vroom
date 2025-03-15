#pragma once

#include <entt/entt.hpp>

#include "Vroom/Scene/Scripting/ScriptEngine.h"
#include "Vroom/Scene/Scripting/ScriptFactory.h"

namespace vrm
{

class Scene;
class Entity;

class ScriptComponent
{
public:
    friend Entity;
public:
    ScriptComponent() = default;
    ScriptComponent(const ScriptComponent&) = delete;
    ScriptComponent& operator=(const ScriptComponent&) = delete;
    ScriptComponent(ScriptComponent&&) = delete;
    ScriptComponent& operator=(ScriptComponent&&) = delete;
    virtual ~ScriptComponent() = default;

    virtual void onSpawn() {}
    virtual void onUpdate(float dt) {}
    virtual void onDestroy() {}

protected:
    Entity getEntity() const;
    Scene& getScene() const;

private:
    void setEntityHandle(entt::entity handle);

private:
    entt::entity m_EntityHandle = entt::null;
};

#define VRM_SCRIPT(ScriptClass) \
  namespace vrm\
  {\
    class ScriptClass##_Factory : public ScriptFactory\
    {\
    public:\
      ScriptClass##_Factory() = default;\
      ~##ScriptClass##_Factory() = default;\
      virtual [[nodiscard]] ScriptClass* create() const override\
      {\
        return new ScriptClass();\
      }\
\
      virtual [[nodiscard]] ScriptClass##_Factory* clone() const override\
      {\
        return new ScriptClass##_Factory();\
      }\
    };\
\
    struct ScriptClass##_Registerer_t\
    {\
      inline ScriptClass##_Registerer_t()\
      {\
        ScriptEngine::Get().registerScript(VRM_GEN_SCRIPT_ID(ScriptClass), std::make_unique<ScriptClass##_Factory>());\
      }\
    };\
\
    inline ScriptClass##_Registerer_t VRM_ ## ScriptClass ## _Registerer;\
  \
  }\


} // namespace vrm
