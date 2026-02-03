#pragma once

#include <entt/entt.hpp>

#include "Vroom/Api.h"

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

  ScriptComponent()                                  = default;
  ScriptComponent(const ScriptComponent&)            = delete;
  ScriptComponent& operator=(const ScriptComponent&) = delete;
  ScriptComponent(ScriptComponent&&)                 = delete;
  ScriptComponent& operator=(ScriptComponent&&)      = delete;
  virtual ~ScriptComponent()                         = default;

  virtual void onSpawn()
  {}
  virtual void onUpdate(const DeltaTime& dt)
  {}
  virtual void onDestroy()
  {}

  inline const std::string& getScriptName() const
  {
    return m_scriptName;
  }

protected:

  Entity getEntity() const;
  Scene& getScene() const;

private:

  void setEntityHandle(entt::entity handle);
  void setSceneRef(Scene* scene)
  {
    m_Scene = scene;
  }

private:

  std::string  m_scriptName;
  entt::entity m_EntityHandle = entt::null;
  Scene*       m_Scene        = nullptr;
};

template <typename ScriptClass>
struct ScriptComponentTraits
{
  inline static constexpr std::string_view scriptId = "";
};

} // namespace vrm
