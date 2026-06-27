#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include "Physics/PhysicsLayerList.h"

namespace vrm
{

class JoltObjectLayerPairFilter : public JPH::ObjectLayerPairFilter
{
public:

  /**
   * @brief 
   * 
   * @param layerList A (STORED) PhysicsLayerList
   */
  JoltObjectLayerPairFilter(const PhysicsLayerList& layerList);

  virtual bool ShouldCollide(JPH::ObjectLayer layer1, JPH::ObjectLayer layer2) const override;

private:

  const PhysicsLayerList& m_layerList;
};

} // namespace vrm
