#include "VroomEditor/TestScene/RotatingDirLightScript.h"

#include <cmath>

#include "Vroom/Scene/Entity.h"
#include "Vroom/Core/DeltaTime.h"
#include "Vroom/Scene/Components/TransformComponent.h"

void RotatingDirLightScript::onSpawn()
{
  auto& tc = getEntity().getComponent<vrm::TransformComponent>();

  m_angle = tc.getRotation().y;
}

void RotatingDirLightScript::onUpdate(const vrm::DeltaTime& dt)
{
  return;
  m_angle += dt.seconds() * m_rotSpeed;
  m_angle = std::fmod(m_angle, glm::radians(360.f));
  auto& tc = getEntity().getComponent<vrm::TransformComponent>();
  auto rot = tc.getRotation();
  rot.y = m_angle;
  tc.setRotation(rot);
}
