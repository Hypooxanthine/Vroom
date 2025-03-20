#pragma once

#include <unordered_set>

#include "Vroom/Scene/Entity.h"

namespace vrm
{

struct HierarchyComponent
{
  Entity parent;
  std::unordered_set<Entity> children;
};

} // namespace vrm
