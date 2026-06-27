#include "Physics/Physics.h"

#include <cassert>
#include <memory>

#include <Jolt/Jolt.h>

#include "Jolt/Core/Factory.h"
#include "Jolt/Core/Memory.h"
#include "Jolt/RegisterTypes.h"
#include "Physics/JoltPhysics.h"
#include "Physics/PhysicsSettings.h"

using namespace vrm;

namespace
{

static std::unique_ptr<Physics> s_physicsInstance = nullptr;

}

void Physics::Init()
{
  JPH::RegisterDefaultAllocator();
  JPH::Factory::sInstance = new JPH::Factory();
  JPH::RegisterTypes();

  s_physicsInstance.reset(new Physics());
}

void Physics::Shutdown()
{
  s_physicsInstance.reset();
  
	JPH::UnregisterTypes();
  delete JPH::Factory::sInstance;
  JPH::Factory::sInstance = nullptr;
}

Physics& Physics::Get()
{
  return *s_physicsInstance;
}

Physics::Physics()
{}

JoltPhysics& Physics::getImplementation() const
{
  assert(m_implementation.get() != nullptr && "Physics module is not configured yet");
  return *m_implementation;
}

const PhysicsSettings& Physics::getSettings() const
{
  return m_settings.value();
}

void Physics::reconfigure(const PhysicsSettings& settings)
{
  m_settings = settings;
  m_implementation.reset(new JoltPhysics(*m_settings));
}
