#pragma once

#include <Vroom/Scene/Components/ScriptComponent.h>

class SuzanneScript : public vrm::ScriptComponent
{
public:
  SuzanneScript() = default;
  SuzanneScript(float circleRadius, float startAngle, float speed);
  ~SuzanneScript() = default;

  void onSpawn() override;

  void onUpdate(float dt) override;

private:
  void updatePosition(float dt);

private:
  float m_CircleRadius = 0.f;
  float m_Angle = 0.f;
  float m_Speed = 0.f;
};

VRM_SCRIPT(SuzanneScript)
