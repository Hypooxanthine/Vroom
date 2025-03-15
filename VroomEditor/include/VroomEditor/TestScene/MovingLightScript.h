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
  float m_CircleRadius = 0.f;
  float m_Height = 0.f;
  float m_Angle = 0.f;
  float m_Speed = 0.f;
};

VRM_SCRIPT(MovingLightScript)
