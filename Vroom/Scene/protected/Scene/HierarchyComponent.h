#pragma once

#include <list>

#include "Scene/Entity.h"
#include "Scene/NameComponent.h"

namespace vrm
{

struct HierarchyComponent
{
  Entity parent;
  std::list<Entity> children;
};

} // namespace vrm
