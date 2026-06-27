#include "Physics/PhysicsLayer.h"

#include <cassert>

using namespace vrm;

PhysicsLayer::PhysicsLayer(EPhysicsLayerGroup group) : m_group(group)
{}

EPhysicsLayerGroup PhysicsLayer::getGroup() const
{
  return m_group;
}

size_t PhysicsLayer::getId() const
{
  assert(m_id.has_value() && "This layer has no ID : it should have been added to a LayerList first.");
  return *m_id;
}

bool PhysicsLayer::collides(const PhysicsLayer& otherLayer) const
{
  return collides(otherLayer.getId());
}

bool PhysicsLayer::collides(size_t otherLayderId) const
{
  assert(m_collisionMap.has_value() && "This layer has no collision map : it should have been added to a LayerList first.");
  return m_collisionMap->at(otherLayderId);
}
