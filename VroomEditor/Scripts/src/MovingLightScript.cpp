#include "Scripts/MovingLightScript.h"

#include <Vroom/Core/DeltaTime.h>
#include <Vroom/Core/Log.h>
#include <Vroom/Scene/Components/TransformComponent.h>
#include <Vroom/Scene/Entity.h>


void MovingLightScript::onSpawn()
{
  const auto& transform = getEntity().getComponent<vrm::TransformComponent>();
  const auto& pos       = transform.getPosition();

  m_CircleRadius = glm::length(glm::vec2(pos.x, pos.z));
  m_Height       = pos.y;
  m_Angle        = atan2(pos.z, pos.x);
}

void MovingLightScript::onUpdate(const vrm::DeltaTime& dt)
{
  updatePosition(dt);
}

void MovingLightScript::updatePosition(const vrm::DeltaTime& dt)
{
  m_Angle += m_Speed * dt.seconds();

  auto& transform = getEntity().getComponent<vrm::TransformComponent>();
  // For the height, we add a bit of oscillation
  float height = m_Height + 0.54553f * sin(m_Angle * 12.2458f);
  transform.setPosition({ m_CircleRadius * cos(m_Angle), height, m_CircleRadius * sin(m_Angle) });
}
