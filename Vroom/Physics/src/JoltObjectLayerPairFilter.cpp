#include "Physics/JoltObjectLayerPairFilter.h"

#include <cassert>

using namespace vrm;

JoltObjectLayerPairFilter::JoltObjectLayerPairFilter(const PhysicsLayerList& layerList) : m_layerList(layerList)
{}

bool JoltObjectLayerPairFilter::ShouldCollide(JPH::ObjectLayer layer1, JPH::ObjectLayer layer2) const
{
  size_t layer1Id = layer1;
  size_t layer2Id = layer2;
  assert(layer1Id < m_layerList.getLayerCount());
  
  return m_layerList.getLayers().at(layer1).collides(layer2Id);
}
