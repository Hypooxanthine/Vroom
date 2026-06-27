#include "Physics/JoltObjectVsBroadPhaseLayerFilter.h"

#include "Jolt/Core/IssueReporting.h"
#include "Physics/PhysicsLayerGroups.h"

using namespace vrm;

JoltObjectVsBroadPhaseLayerFilter::JoltObjectVsBroadPhaseLayerFilter(const JoltBroadPhaseLayerInterface& bplInterface)
  : m_bplInterface(bplInterface)
{}

bool JoltObjectVsBroadPhaseLayerFilter::ShouldCollide(JPH::ObjectLayer objectLayer, JPH::BroadPhaseLayer bpLayer) const
{
  switch ((EPhysicsLayerGroup)m_bplInterface.GetBroadPhaseLayer(objectLayer).GetValue())
  {
  case EPhysicsLayerGroup::eStatic:
    return (EPhysicsLayerGroup)bpLayer.GetValue() == EPhysicsLayerGroup::eDynamic;
  case EPhysicsLayerGroup::eDynamic:
    return true;
  default:
    JPH_ASSERT(false && "Object layer has an unknown BroadPhaseLayer");
    return false;
  }
}
