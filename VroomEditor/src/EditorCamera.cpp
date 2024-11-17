#include "VroomEditor/EditorCamera.h"

#include <Vroom/Core/Application.h>

namespace vrm
{

EditorCamera::EditorCamera(float viewNear, float viewFar, float viewFov, float viewAspectRatio, const glm::vec3& position, const glm::vec3& rotation)
    : FirstPersonCamera(viewNear, viewFar, viewFov, viewAspectRatio, position, rotation)
{
}

void EditorCamera::onUpdate(float dt)
{
    move(m_MoveForwardValue * m_Speed * dt * getForwardVector());
    move(m_MoveRightValue * m_Speed * dt * getRightVector());
    move(m_MoveUpValue * m_Speed * dt * glm::vec3{0.f, 1.f, 0.f});
    addYaw(m_LookRightValue * m_AngularSpeed);
    addPitch(m_LookUpValue * m_AngularSpeed);

    m_LookUpValue = 0.f;
    m_LookRightValue = 0.f;
}

void EditorCamera::submitLookUp(float value)
{
    m_LookUpValue = value;
}

void EditorCamera::submitLookRight(float value)
{
    m_LookRightValue = value;
}

void EditorCamera::addMoveForward(float value)
{
    m_MoveForwardValue += value;
}

void EditorCamera::addMoveRight(float value)
{
    m_MoveRightValue += value;
}

void EditorCamera::addMoveUp(float value)
{
    m_MoveUpValue += value;
}

} // namespace vrm