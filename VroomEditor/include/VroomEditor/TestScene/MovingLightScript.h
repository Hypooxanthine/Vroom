#pragma once

#include <Vroom/Scene/Components/ScriptComponent.h>

class MovingLightScript : public vrm::ScriptComponent
{
public:
  MovingLightScript() = default;
  MovingLightScript(float circleRadius, float height, float startAngle, float speed);
  ~MovingLightScript() = default;

  void onSpawn() override;

  void onUpdate(float dt) override;

private:
  void updatePosition(float dt);

private:
  float m_CircleRadius = 10.f;
  float m_Height = 10.f;
  float m_Angle = 0.f;
  float m_Speed = -3.14159f / 4.f;
};

VRM_SCRIPT(MovingLightScript)
