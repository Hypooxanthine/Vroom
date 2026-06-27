#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>
#include "JoltBroadPhaseLayerInterface.h"

namespace vrm
{

class JoltObjectVsBroadPhaseLayerFilter : public JPH::ObjectVsBroadPhaseLayerFilter
{
public:

  /**
   * @brief Constructor
   * @warning The bplInterface must be kept alive until the destruction of the JoltObjectVsBroadPhaseLayerFilter
   * 
   * @param bplInterface A (STORED) reference on a JoltObjectVsBroadPhaseLayerFilter
   */
  JoltObjectVsBroadPhaseLayerFilter(const JoltBroadPhaseLayerInterface& bplInterface);

  virtual bool ShouldCollide(JPH::ObjectLayer objectLayer, JPH::BroadPhaseLayer bpLayer) const override;

private:

  const JoltBroadPhaseLayerInterface& m_bplInterface;
};

} // namespace vrm
