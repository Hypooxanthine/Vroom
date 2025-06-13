#include "Scripts/SuzanneScript.h"

#include <Vroom/Core/DeltaTime.h>
#include <Vroom/Scene/Entity.h>
#include <Vroom/Scene/Components/TransformComponent.h>

SuzanneScript::SuzanneScript(float circleRadius, float startAngle, float speed)
  : vrm::ScriptComponent(), m_CircleRadius(circleRadius), m_Angle(startAngle), m_Speed(speed)
{
}

void SuzanneScript::onSpawn()
{
  m_Angle = getEntity().getComponent<vrm::TransformComponent>().getRotation().y;
  updatePosition({});
}

void SuzanneScript::onUpdate(const vrm::DeltaTime& dt)
{
  updatePosition(dt);
}

void SuzanneScript::updatePosition(const vrm::DeltaTime& dt)
{
  m_Angle += m_Speed * dt.seconds();

  auto& transform = getEntity().getComponent<vrm::TransformComponent>();
  transform.setPosition({ m_CircleRadius * cos(m_Angle), transform.getPosition().y, m_CircleRadius * sin(m_Angle) });
  transform.setRotation(glm::vec3{ transform.getRotation().x , -m_Angle, transform.getRotation().z });
}