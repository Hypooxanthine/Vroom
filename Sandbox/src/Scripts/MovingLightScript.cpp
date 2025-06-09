#include "Scripts/MovingLightScript.h"

#include <Vroom/Core/Log.h>
#include <Vroom/Scene/Entity.h>
#include <Vroom/Scene/Components/TransformComponent.h>
#include <Vroom/Core/DeltaTime.h>

MovingLightScript::MovingLightScript(float circleRadius, float height, float startAngle, float speed)
    : vrm::ScriptComponent(), m_CircleRadius(circleRadius), m_Height(height), m_Angle(startAngle), m_Speed(speed)
{
}

MovingLightScript::~MovingLightScript()
{
}

void MovingLightScript::onSpawn()
{
}

void MovingLightScript::onUpdate(const vrm::DeltaTime& dt)
{
    m_Angle += m_Speed * dt.seconds();

    auto& transform = getEntity().getComponent<vrm::TransformComponent>();
    // For the height, we add a bit of oscillation
    float height = m_Height + 0.54553f * sin(m_Angle * 12.2458f);
    transform.setPosition({ m_CircleRadius * cos(m_Angle), height, m_CircleRadius * sin(m_Angle) });
}

void MovingLightScript::onDestroy()
{
}