#pragma once

#include <Vroom/Render/Camera/FirstPersonCamera.h>

#include <glm/gtc/constants.hpp>

namespace vrm
{

class DeltaTime;

class EditorCamera : public FirstPersonCamera
{
public:
    EditorCamera(float viewNear, float viewFar, float viewFov, float viewAspectRatio, const glm::vec3& position, const glm::vec3& rotation);

    void onUpdate(const DeltaTime& dt);

    void submitLookUp(float value);
    void submitLookRight(float value);
    void addMoveForward(float value);
    void addMoveRight(float value);
    void addMoveUp(float value);

    void clearInputs();

    inline void setSpeed(float speed) { m_Speed = speed; }
    inline void setAngularSpeed(float angularSpeed) { m_AngularSpeed = angularSpeed; }

    inline float getSpeed() const { return m_Speed; }
    inline float getAngularSpeed() const { return m_AngularSpeed; }

private:
    float m_LookUpValue = 0.f, m_LookRightValue = 0.f;
    float m_MoveForwardValue = 0.f, m_MoveRightValue = 0.f, m_MoveUpValue = 0.f;
    float m_Speed = 10.f, m_AngularSpeed = 3.f;
};

} // namespace vrm
