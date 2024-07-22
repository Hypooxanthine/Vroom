#pragma once

#include <entt/entt.hpp>

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

} // namespace vrm