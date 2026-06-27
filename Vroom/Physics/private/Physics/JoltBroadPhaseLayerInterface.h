#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>

#include "Physics/typedefs.h"

namespace vrm
{

class JoltBroadPhaseLayerInterface : public JPH::BroadPhaseLayerInterface
{
public:

  /**
   * @brief Tells Jolt the BroadPhaseLayer of every ObjectLayer
   *
   */
  JoltBroadPhaseLayerInterface(const PhysicsLayerList& physicsLayers);

  virtual JPH::uint GetNumBroadPhaseLayers() const override;

  virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override;

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
  virtual const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override;
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

private:

  std::vector<JPH::BroadPhaseLayer> m_objectLayerToBroadPhaseLayer;
};

} // namespace vrm
