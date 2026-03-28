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

  /**
   * @brief Called when the script is created. The scene is not necessarly playing,
   for example in the editor, when the script is added to an entity manually or when
   a scene is loaded.
   * 
   */
  virtual void onInit()
  {}

  /**
   * @brief Called when the scene started playing or when the script
   is added to an entity in a playing scene.
   * 
   */
  virtual void onSpawn()
  {}

  /**
   * @brief Called once per frame by a playing scene.
   * 
   * @param dt Ellapsed time since the last frame.
   */
  virtual void onUpdate(const DeltaTime& dt)
  {}

  /**
   * @brief When the script is destroyed
   * 
   */
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
