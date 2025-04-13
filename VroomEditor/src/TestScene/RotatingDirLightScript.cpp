#include "VroomEditor/TestScene/RotatingDirLightScript.h"

#include "Vroom/Scene/Entity.h"
#include "Vroom/Scene/Components/TransformComponent.h"

void RotatingDirLightScript::onSpawn()
{
  auto& tc = getEntity().getComponent<vrm::TransformComponent>();

  m_angle = tc.getRotation().z;
}

void RotatingDirLightScript::onUpdate(float dt)
{
  m_angle += dt * m_rotSpeed;
  m_angle = std::fmodf(m_angle, glm::radians(360.f));
  auto& tc = getEntity().getComponent<vrm::TransformComponent>();
  auto rot = tc.getRotation();
  rot.z = m_angle;
  tc.setRotation(rot);
}
