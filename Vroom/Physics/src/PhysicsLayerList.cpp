#include "Physics/PhysicsLayerList.h"
#include <cassert>
#include "Physics/PhysicsLayer.h"

using namespace vrm;

PhysicsLayerList::PhysicsLayerList()
{}

const std::vector<PhysicsLayer>& PhysicsLayerList::getLayers() const
{
  return m_layers;
}

size_t PhysicsLayerList::getLayerCount() const
{
  return m_layers.size();
}

size_t PhysicsLayerList::addLayer(PhysicsLayer&& layer)
{
  size_t id = m_layers.size();
  m_layers.emplace_back(std::move(layer));
  m_layers.back().m_id = id;
  return id;
}

void PhysicsLayerList::generateCollisionMaps()
{
  for (PhysicsLayer& layer : m_layers)
  {
    // Default is no collision at all
    layer.m_collisionMap = std::vector<bool>(m_layers.size(), false);
  }
}

void PhysicsLayerList::setColliding(size_t layerId1, size_t layerId2, bool colliding)
{
  assert(layerId1 < m_layers.size() && layerId2 < m_layers.size());

  m_layers.at(layerId1).m_collisionMap->at(layerId2) = colliding;
  m_layers.at(layerId2).m_collisionMap->at(layerId1) = colliding;
}
