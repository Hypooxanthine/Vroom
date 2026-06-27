#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>

#include "JoltBroadPhaseLayerInterface.h"
#include "JoltObjectLayerPairFilter.h"
#include "JoltObjectVsBroadPhaseLayerFilter.h"
#include "Physics/PhysicsSettings.h"
#include "Physics/typedefs.h"

namespace vrm
{

class JoltPhysics
{
public:

  /**
   * @brief
   *
   * @param settings A (STORED) ref to the settings of the Physics module
   */
  JoltPhysics(const PhysicsSettings& settings);

private:

  const PhysicsSettings& m_settings;

  JoltBroadPhaseLayerInterface      m_bplInterface;
  JoltObjectVsBroadPhaseLayerFilter m_objVsBpLayerFilter;
  JoltObjectLayerPairFilter         m_objLayerPairFilter;
  JPH::PhysicsSystem                m_system;
};

}; // namespace vrm
