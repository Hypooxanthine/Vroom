#pragma once

#include <Vroom/Scene/Components/ScriptComponent.h>

class SuzanneScript : public vrm::ScriptComponent
{
public:
  SuzanneScript() = default;
  SuzanneScript(float circleRadius, float startAngle, float speed);
  ~SuzanneScript() = default;

  void onSpawn() override;

  void onUpdate(const vrm::DeltaTime& dt) override;

private:
  void updatePosition(const vrm::DeltaTime& dt);

private:
  float m_CircleRadius = 10.f;
  float m_Angle = 0.f;
  float m_Speed = 3.14159f / 4.f;
};

VRM_SCRIPT(SuzanneScript)
