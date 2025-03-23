#pragma once

#include <list>

#include "Vroom/Scene/Entity.h"
#include "Vroom/Scene/Components/NameComponent.h"

namespace vrm
{

struct HierarchyComponent
{
  Entity parent;
  std::list<Entity> children;
};

} // namespace vrm
