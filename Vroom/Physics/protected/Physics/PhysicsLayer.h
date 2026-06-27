#pragma once

#include <cstddef>
#include <optional>
#include <vector>

#include "Physics/PhysicsLayerGroups.h"
#include "Physics/typedefs.h"

namespace vrm
{

class PhysicsLayer
{
public:

  PhysicsLayer(EPhysicsLayerGroup group);

  EPhysicsLayerGroup getGroup() const;
  size_t             getId() const;
  bool               collides(const PhysicsLayer& otherLayer) const;
  bool               collides(size_t otherLayderId) const;

private:

  EPhysicsLayerGroup m_group;

  // To be set by the PhysicsLayerList
  friend PhysicsLayerList;
  std::optional<size_t>            m_id;
  std::optional<std::vector<bool>> m_collisionMap;
};

} // namespace vrm
