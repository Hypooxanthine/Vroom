#include "VroomEditor/TestScene/SuzanneScript.h"

#include <Vroom/Scene/Entity.h>
#include <Vroom/Scene/Components/TransformComponent.h>

SuzanneScript::SuzanneScript(float circleRadius, float startAngle, float speed)
    : vrm::ScriptComponent(), m_CircleRadius(circleRadius), m_Angle(startAngle), m_Speed(speed)
{
}

void SuzanneScript::onUpdate(float dt)
{
    m_Angle += m_Speed * dt;

    auto& transform = getEntity().getComponent<vrm::TransformComponent>();
    transform.setPosition({ m_CircleRadius * cos(m_Angle), 0.f, m_CircleRadius * sin(m_Angle) });
    transform.setRotation({ 0.f, -m_Angle, 0.f });
}
