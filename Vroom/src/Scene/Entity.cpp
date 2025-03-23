#include "Vroom/Scene/Entity.h"

#include "Vroom/Core/Application.h"
#include "Vroom/Core/GameLayer.h"
#include "Vroom/Scene/Scene.h"

namespace vrm
{

Entity::Entity(entt::entity handle, entt::registry* registry, Scene* scene)
    : m_Handle(handle), m_Scene(scene)
{
}

Entity::Entity(const Entity& other)
    : m_Handle(other.m_Handle), m_Scene(other.m_Scene)
{
}

Entity& Entity::operator=(const Entity& other)
{
    if (this != &other)
    {
        m_Handle = other.m_Handle;
        m_Scene = other.m_Scene;
    }

    return *this;
}

Entity::Entity(Entity&& other)
    : m_Handle(other.m_Handle), m_Scene(other.m_Scene)
{
    other.m_Handle = entt::null;
    other.m_Scene = nullptr;
}

Entity& Entity::operator=(Entity&& other)
{
    if (this != &other)
    {
        m_Handle = other.m_Handle;
        m_Scene = other.m_Scene;

        other.m_Handle = entt::null;
        other.m_Scene = nullptr;
    }

    return *this;
}

entt::registry& Entity::getEnttRegistry()
{
  return m_Scene->getRegistry();
}

const entt::registry& Entity::getEnttRegistry() const
{
  return const_cast<Entity*>(this)->getEnttRegistry();
}

bool Entity::isValid() const
{
  return
      m_Handle != entt::null
  &&  m_Scene == &Application::Get().getGameLayer().getScene()
  &&  m_Scene->getRegistry().valid(m_Handle);
}

} // namespace vrm