#include "Vroom/Scene/Entity.h"

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

} // namespace vrm