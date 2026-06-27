#include "Physics/JoltPhysics.h"
#include "Physics/PhysicsSettings.h"

using namespace vrm;

JoltPhysics::JoltPhysics(const PhysicsSettings& settings)
  : m_settings(settings)
  , m_bplInterface(settings.layers)
  , m_objVsBpLayerFilter(m_bplInterface)
  , m_objLayerPairFilter(m_settings.layers)
{
  m_system.Init(m_settings.maxBodies, 0, m_settings.maxBodyPairs, m_settings.maxContactConstraints, m_bplInterface, m_objVsBpLayerFilter, m_objLayerPairFilter);
}
