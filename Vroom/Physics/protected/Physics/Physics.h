#pragma once

#include <memory>
#include <optional>

#include "Physics/Api.h"
#include "Physics/PhysicsSettings.h"
#include "Physics/typedefs.h"

namespace vrm
{

class Physics
{
public:

  VRM_PHYSICS_API static void Init();
  VRM_PHYSICS_API static void Shutdown();

  VRM_PHYSICS_API static Physics& Get();

  // For internal use of the Physics module.
  VRM_PHYSICS_API JoltPhysics& getImplementation() const;

  /**
   * @brief Get the settings of the Physics system.
   * @warning Could be unsafe since it is dereferencing an optional, but should never crash if correctly configured
   * before this function is ever called.
   */
  VRM_PHYSICS_API const PhysicsSettings& getSettings() const;

  /**
   * @brief Recongigures the physics system.
   * @warning Everything is trashed ! The simulation has to be entirely filled again after a reconfiguration.
   */
  VRM_PHYSICS_API void reconfigure(const PhysicsSettings& settings);

private:

  Physics();

private:

  // Maybe not configured yet (should be)
  std::optional<PhysicsSettings> m_settings;

  // Jolt is the backend of the Physics module and
  // is not accessible from outside this module.
  std::unique_ptr<JoltPhysics> m_implementation;
};

} // namespace vrm
