#include "VroomEditor/TestScene/SuzanneScript.h"

#include <Vroom/Scene/Entity.h>
#include <Vroom/Scene/Components/TransformComponent.h>

SuzanneScript::SuzanneScript(float circleRadius, float startAngle, float speed)
  : vrm::ScriptComponent(), m_CircleRadius(circleRadius), m_Angle(startAngle), m_Speed(speed)
{
}

void SuzanneScript::onSpawn()
{
  m_Angle = getEntity().getComponent<vrm::TransformComponent>().getRotation().y;
  updatePosition(0.f);
}

void SuzanneScript::onUpdate(float dt)
{
  updatePosition(dt);
}

void SuzanneScript::updatePosition(float dt)
{
  m_Angle += m_Speed * dt;

  auto& transform = getEntity().getComponent<vrm::TransformComponent>();
  transform.setPosition({ m_CircleRadius * cos(m_Angle), transform.getPosition().y, m_CircleRadius * sin(m_Angle) });
  transform.setRotation(glm::vec3{ transform.getRotation().x , -m_Angle, transform.getRotation().z });
}