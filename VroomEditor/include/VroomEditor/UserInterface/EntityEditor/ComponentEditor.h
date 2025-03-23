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

  static void EditEntity(Entity& e);
};

} // namespace vrm
