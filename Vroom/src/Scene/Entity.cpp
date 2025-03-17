#include "Vroom/Scene/Entity.h"

namespace vrm
{

Entity::Entity(entt::entity handle, entt::registry* registry, Scene* scene)
    : m_Handle(handle), m_Registry(registry), m_Scene(scene)
{
}

Entity::Entity(const Entity& other)
    : m_Handle(other.m_Handle), m_Registry(other.m_Registry), m_Scene(other.m_Scene)
{
}

Entity& Entity::operator=(const Entity& other)
{
    if (this != &other)
    {
        m_Handle = other.m_Handle;
        m_Registry = other.m_Registry;
        m_Scene = other.m_Scene;
    }

    return *this;
}

Entity::Entity(Entity&& other)
    : m_Handle(other.m_Handle), m_Registry(other.m_Registry), m_Scene(other.m_Scene)
{
    other.m_Handle = entt::null;
    other.m_Registry = nullptr;
    other.m_Scene = nullptr;
}

Entity& Entity::operator=(Entity&& other)
{
    if (this != &other)
    {
        m_Handle = other.m_Handle;
        m_Registry = other.m_Registry;
        m_Scene = other.m_Scene;

        other.m_Handle = entt::null;
        other.m_Registry = nullptr;
        other.m_Scene = nullptr;
    }

    return *this;
}

} // namespace vrm