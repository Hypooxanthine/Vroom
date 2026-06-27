#include "Physics/JoltBroadPhaseLayerInterface.h"
#include <cstdint>

#include "Jolt/Core/IssueReporting.h"
#include "Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h"
#include "Physics/PhysicsLayerGroups.h"
#include "Physics/PhysicsLayerList.h"

using namespace vrm;

JoltBroadPhaseLayerInterface::JoltBroadPhaseLayerInterface(const PhysicsLayerList& layerList)
{
  for (const PhysicsLayer& layer : layerList.getLayers())
  {
    JPH::BroadPhaseLayer broadPhaseLayer((uint8_t)layer.getGroup());
    m_objectLayerToBroadPhaseLayer.emplace_back(broadPhaseLayer);
  }
}

JPH::uint JoltBroadPhaseLayerInterface::GetNumBroadPhaseLayers() const
{
  return (JPH::uint)EPhysicsLayerGroup::Count;
}

JPH::BroadPhaseLayer JoltBroadPhaseLayerInterface::GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const
{
  JPH_ASSERT(inLayer < m_objectLayerToBroadPhaseLayer.size());

  return m_objectLayerToBroadPhaseLayer.at(inLayer);
}

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
const char* JoltBroadPhaseLayerInterface::GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const
{
  switch ((EPhysicsLayerGroup)inLayer.GetValue())
  {
  case EPhysicsLayerGroup::eStatic:
    return "Static";
  case EPhysicsLayerGroup::eDynamic:
    return "Dynamic";
  default:
    JPH_ASSERT(false);
    return "Unknown";
  }
}
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED
