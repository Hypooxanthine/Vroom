#include "Vroom/Scene/Entity.h"

namespace vrm
{

Entity::Entity(entt::entity handle, entt::registry* registry)
    : m_Handle(handle), m_Registry(registry)
{
}

Entity::Entity(const Entity& other)
    : m_Handle(other.m_Handle), m_Registry(other.m_Registry)
{
}

Entity& Entity::operator=(const Entity& other)
{
    if (this != &other)
    {
        m_Handle = other.m_Handle;
        m_Registry = other.m_Registry;
    }

    return *this;
}

Entity::Entity(Entity&& other)
    : m_Handle(other.m_Handle), m_Registry(other.m_Registry)
{
    other.m_Handle = entt::null;
    other.m_Registry = nullptr;
}

Entity& Entity::operator=(Entity&& other)
{
    if (this != &other)
    {
        m_Handle = other.m_Handle;
        m_Registry = other.m_Registry;

        other.m_Handle = entt::null;
        other.m_Registry = nullptr;
    }

    return *this;
}

} // namespace vrm