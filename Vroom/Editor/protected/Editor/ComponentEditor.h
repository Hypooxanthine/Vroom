#pragma once

#include <Scene/Entity.h>

namespace vrm
{

class ComponentEditor
{
public:

  friend class EntityEditor;

  virtual ~ComponentEditor() {}

  virtual void editEntityComponent(Entity& e) const = 0;

  virtual bool canEditEntity(const Entity& e) const = 0;

  virtual void removeFromEntity(Entity& e) const {};

  virtual bool canBeRemoved() const = 0;

  virtual std::string getComponentName() const { return "Component"; }

  static void EditEntity(Entity& e);
};

/**
 * @brief Base class for editors of a single component type.
 *
 * Implements the mechanical parts of the ComponentEditor interface that only
 * depend on the component type (presence check and removal). Concrete editors
 * derive from this and only have to provide getComponentName() and
 * editEntityComponent().
 *
 * @tparam Component The edited component type.
 * @tparam Removable Whether the component can be removed from an entity.
 */
template <typename Component, bool Removable>
class TComponentEditor : public ComponentEditor
{
public:

  bool canEditEntity(const Entity& e) const override
  {
    return e.hasComponent<Component>();
  }

  bool canBeRemoved() const override
  {
    return Removable;
  }

  void removeFromEntity(Entity& e) const override
  {
    if constexpr (Removable)
      e.removeComponent<Component>();
  }

protected:

  Component& get(Entity& e) const
  {
    return e.getComponent<Component>();
  }
};

} // namespace vrm
