#pragma once

#include <entt/entt.hpp>

#include "Vroom/Core/Assert.h"

namespace vrm
{

/**
 * @brief Entity class.
 * 
 * An entity is a simple container for components.
 * It is a simple wrapper around an entt::entity.
 */
class Entity
{
public:
    Entity() = default;

    /**
     * @brief Constructor.
     * 
     * @param handle The entity handle.
     * @param registry The registry where the entity is stored.
     */
    Entity(entt::entity handle, entt::registry* registry);
    
    /**
     * @brief Copy constructor.
     * @warning It does not create a new entity, but a new reference to the same entity.
     * 
     */
    Entity(const Entity&);

    /**
     * @brief Copy assignment operator.
     * @warning It does not create a new entity, but a new reference to the same entity.
     * 
     */
    Entity& operator=(const Entity&);

    /**
     * @brief Move constructor.
     * 
     */
    Entity(Entity&& other);

    /**
     * @brief Move assignment operator.
     * 
     */
    Entity& operator=(Entity&& other);
    
    ~Entity() = default;

    /**
     * @brief Add a component to the entity.
     * 
     * @tparam T The type of the component.
     * @tparam Args The types of the arguments to pass to the component constructor.
     * @param args The arguments to pass to the component constructor.
     * @return T& The reference to the component.
     */
    template<typename T, typename... Args>
    T& addComponent(Args&&... args)
    {
        VRM_ASSERT_MSG(!hasComponent<T>(), "Entity already has component.");
        return m_Registry->emplace<T>(m_Handle, std::forward<Args>(args)...);
    }

    /**
     * @brief Get a component from the entity.
     * 
     * @tparam T The type of the component.
     * @return T& The reference to the component.
     */
    template<typename T>
    T& getComponent()
    {
        VRM_ASSERT_MSG(hasComponent<T>(), "Entity does not have component.");
        return m_Registry->get<T>(m_Handle);
    }

    /**
     * @brief Check if the entity has a component.
     * 
     * @tparam T The type of the component.
     * @return true If the entity has the component.
     * @return false If the entity does not have the component.
     */
    template<typename T>
    bool hasComponent()
    {
        return m_Registry->try_get<T>(m_Handle) != nullptr;
    }

    /**
     * @brief Remove a component from the entity.
     * 
     * @tparam T The type of the component.
     */
    template<typename T>
    void removeComponent()
    {
        VRM_ASSERT_MSG(hasComponent<T>(), "Entity does not have component.");
        m_Registry->remove<T>(m_Handle);
    }

    operator bool() const { return m_Handle != entt::null; }

    operator entt::entity() const { return m_Handle; }

    bool operator==(const Entity& other) const { return m_Handle == other.m_Handle && m_Registry == other.m_Registry; }

    bool operator!=(const Entity& other) const { return !(*this == other); }

private:
    entt::entity m_Handle = entt::null;
    entt::registry* m_Registry = nullptr;
};

} // namespace vrm
