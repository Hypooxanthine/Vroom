#include "Scripts/SuzanneScript.h"

#include <Vroom/Core/Log.h>
#include <Vroom/Core/DeltaTime.h>
#include <Vroom/Scene/Entity.h>
#include <Vroom/Scene/Components/TransformComponent.h>

SuzanneScript::SuzanneScript(float circleRadius, float startAngle, float speed)
    : vrm::ScriptComponent(), m_CircleRadius(circleRadius), m_Angle(startAngle), m_Speed(speed)
{
}

SuzanneScript::~SuzanneScript()
{
}

void SuzanneScript::onSpawn()
{
    VRM_LOG_INFO("Suzanne spawned!");
}

void SuzanneScript::onUpdate(const vrm::DeltaTime& dt)
{
    m_Angle += m_Speed * dt.seconds();

    auto& transform = getEntity().getComponent<vrm::TransformComponent>();
    transform.setPosition({ m_CircleRadius * cos(m_Angle), 0.f, m_CircleRadius * sin(m_Angle) });
    transform.setRotation({ 0.f, -m_Angle, 0.f });
}

void SuzanneScript::onDestroy()
{
    VRM_LOG_INFO("Suzanne destroyed!");
}