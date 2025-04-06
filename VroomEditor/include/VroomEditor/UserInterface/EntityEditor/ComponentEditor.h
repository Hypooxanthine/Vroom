#pragma once

#include <memory>
#include <vector>

#include <Vroom/Scene/Entity.h>

namespace vrm
{

class ComponentEditor
{
public:
  friend class EntityEditor;

  virtual bool editEntityComponent(Entity& e) const = 0;

  virtual bool canEditEntity(const Entity& e) const = 0;

  virtual void removeFromEntity(Entity& e) const {};

  virtual bool canBeRemoved() const = 0;

  virtual std::string getComponentName() const { return "Component"; }

  static void EditEntity(Entity& e);
};

} // namespace vrm
