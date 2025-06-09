#pragma once

#include <glm/glm.hpp>

#include <Vroom/Scene/Components/ScriptComponent.h>

class RotatingDirLightScript : public vrm::ScriptComponent
{
public:
  RotatingDirLightScript() = default;
  ~RotatingDirLightScript() = default;

  void onSpawn() override;

  void onUpdate(const vrm::DeltaTime& dt) override;

private:
  float m_angle = 0.f;
  float m_rotSpeed = glm::radians(90.f);
};

VRM_SCRIPT(RotatingDirLightScript)
