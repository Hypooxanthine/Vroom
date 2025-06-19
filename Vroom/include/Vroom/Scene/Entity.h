#pragma once

#include <entt/entt.hpp>

#include "Vroom/Api.h"
#include "Vroom/Core/Assert.h"
#include "Vroom/Generic/TypeList.h"
#include "Vroom/Scene/Components/ScriptHandler.h"

namespace vrm
{

  class Scene;
  struct NameComponent;
  struct HierarchyComponent;
  class TransformComponent;

  /**
   * @brief Entity class.
   *
   * An entity is a simple container for components.
   * It is a simple wrapper around an entt::entity.
   */
  class VRM_API Entity
  {
  public:
    friend class Scene;
    friend struct std::hash<Entity>;

  public:
    Entity() = default;

    /**
     * @brief Constructor.
     *
     * @param handle The entity handle.
     * @param registry The registry where the entity is stored.
     * @param scene The scene where the entity is.
     */
    Entity(entt::entity handle, entt::registry* registry, Scene* scene);

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
     * @brief This is NOT tagged const, because the newly created Entity instance will access to the same data (components for instance).
     *
     * @return Entity
     */
    Entity clone() { return Entity(*this); }

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
      return getEnttRegistry().emplace<T>(m_Handle, std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    T& addScriptComponent(Args&&... args)
    {
      return static_cast<T&>(addScriptComponent(std::make_unique<T>(std::forward<Args>(args)...)));
    }

    ScriptComponent& addScriptComponent(std::unique_ptr<ScriptComponent>&& script);

    /**
     * @brief Get a component from the entity.
     *
     * @tparam T The type of the component.
     * @return T& The reference to the component.
     */
    template<typename T>
    T& getComponent()
    {
      using UnauthorizedComponents = TypeList<
        NameComponent,
        HierarchyComponent
      >;
      static_assert(
        !UnauthorizedComponents::HasType<T>(),
        "You cannot get a non const reference of this component directly from Entity API."
      );
      return getComponentInternal<T>();
    }

    template<typename T>
    const T& getComponent() const
    {
      return getComponentInternal<T>();
    }

    /**
     * @brief Check if the entity has a component.
     *
     * @tparam T The type of the component.
     * @return true If the entity has the component.
     * @return false If the entity does not have the component.
     */
    template<typename T>
    bool hasComponent() const
    {
      return getEnttRegistry().try_get<T>(m_Handle) != nullptr;
    }

    bool hasScriptComponent() const
    {
      return hasComponent<ScriptHandler>();
    }

    ScriptComponent& getScriptComponent()
    {
      return getComponentInternal<ScriptHandler>().getScript();
    }

    const ScriptComponent& getScriptComponent() const
    {
      return getComponentInternal<ScriptHandler>().getScript();
    }

    /**
     * @brief Remove a component from the entity.
     *
     * @tparam T The type of the component.
     */
    template<typename T>
    void removeComponent()
    {
      using IllegalRemove = TypeList<
        NameComponent,
        HierarchyComponent,
        TransformComponent
      >;
      static_assert(
        !IllegalRemove::HasType<T>(),
        "You cannot remove this component."
      );
      removeComponentInternal<T>();
    }

    const std::string& getName() const;

    void setName(const std::string& name);

    Entity& getParent();
    const Entity& getParent() const;

    std::list<Entity>& getChildren();
    const std::list<Entity>& getChildren() const;

    bool isRoot() const;

    /**
     * @brief Check if the entity is valid.
     *
     * @return true If the entity is valid.
     * @return false If the entity is not valid.
     */
    operator bool() const { return isValid(); }

    /**
     * @brief Equality operator.
     *
     * @param other The entity to compare to.
     * @return true If the entities are equal.
     * @return false If the entities are not equal.
     */
    bool operator==(const Entity& other) const { return m_Handle == other.m_Handle && m_Scene == other.m_Scene; }

    /**
     * @brief Inequality operator.
     *
     * @param other The entity to compare to.
     * @return true If the entities are not equal.
     * @return false If the entities are equal.
     */
    bool operator!=(const Entity& other) const { return !(*this == other); }

    bool isValid() const;

    inline Scene* getScene() const { return m_Scene; }

  private:

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

    template<typename T>
    T& getComponentInternal()
    {
      VRM_ASSERT_MSG(hasComponent<T>(), "Entity does not have component.");
      return getEnttRegistry().get<T>(m_Handle);
    }

    template <typename T>
    const T& getComponentInternal() const
    {
      return const_cast<Entity*>(this)->getComponentInternal<T>();
    }

    template<typename T>
    void removeComponentInternal()
    {
      VRM_ASSERT_MSG(hasComponent<T>(), "Entity does not have component.");
      getEnttRegistry().remove<T>(m_Handle);
    }

    entt::registry& getEnttRegistry();

    const entt::registry& getEnttRegistry() const;

    inline entt::entity getHandle() const { return m_Handle; }

    /**
     * @brief Get the entity handle.
     *
     * @return entt::entity The entity handle.
     */
    operator entt::entity() const { return m_Handle; }

  private:
    entt::entity m_Handle = entt::null;
    Scene* m_Scene = nullptr;
  };

} // namespace vrm

template<>
struct std::hash<vrm::Entity>
{
  size_t operator()(const vrm::Entity& val) const noexcept
  {
    return
      static_cast<size_t>(val.getHandle())
      + static_cast<size_t>(reinterpret_cast<uintptr_t>(val.getScene()));
  }
};
