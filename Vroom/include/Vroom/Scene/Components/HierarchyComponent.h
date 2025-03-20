#pragma once

#include <vector>

#include "Vroom/Scene/Entity.h"

namespace vrm
{

struct HierarchyComponent
{
  Entity parent;
  std::vector<Entity> children;
};

} // namespace vrm
