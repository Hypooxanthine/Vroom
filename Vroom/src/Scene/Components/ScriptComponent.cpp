#include "Vroom/Scene/Components/ScriptComponent.h"

#include "Vroom/Scene/Entity.h"
#include "Vroom/Scene/Scene.h"
#include "Vroom/Core/Application.h"
#include "Vroom/Core/GameLayer.h"

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