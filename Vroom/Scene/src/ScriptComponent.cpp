#include "Scene/ScriptComponent.h"

#include "Scene/Entity.h"
#include "Scene/Scene.h"

namespace vrm
{

Entity ScriptComponent::getEntity() const
{
  return getScene().getEntity(m_EntityHandle);
}

Scene& ScriptComponent::getScene() const
{
  return *m_Scene;
}

void ScriptComponent::setEntityHandle(entt::entity handle)
{
  m_EntityHandle = handle;
}

} // namespace vrm
