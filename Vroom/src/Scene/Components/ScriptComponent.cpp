#include "Vroom/Scene/Components/ScriptComponent.h"

#include "Vroom/Scene/Entity.h"
#include "Vroom/Scene/Scene.h"
#include "Vroom/Core/Application.h"

namespace vrm
{

Entity ScriptComponent::getEntity() const
{
    return Entity(m_EntityHandle, &getScene().getRegistry());
}

Scene& ScriptComponent::getScene() const
{
    return Application::Get().getScene();
}

void ScriptComponent::setEntityHandle(entt::entity handle)
{
    m_EntityHandle = handle;
}

} // namespace vrm