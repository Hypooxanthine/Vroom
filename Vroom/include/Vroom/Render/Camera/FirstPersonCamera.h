#pragma once

#include "Vroom/Render/Camera/PerspectiveCameraBasic.h"

namespace vrm
{

class FirstPersonCamera : public PerspectiveCameraBasic
{
public:
    FirstPersonCamera(float viewNear, float viewFar, float viewFov, float viewAspectRatio, const glm::vec3& position, const glm::vec3& rotation);
    virtual ~FirstPersonCamera() = default;

    /* GETTERS */

    inline glm::vec3 getPosition() const override { return m_WorldPosition; }
    inline const glm::vec3& getRotation() const { return m_Rotation; }

    /* Setters */

    void setWorldPosition(const glm::vec3& pos);
    void setRotation(const glm::vec3& rot);

    /* Modifiers */

    void addYaw(float yaw);
    void addPitch(float pitch);
    void addRoll(float roll);

    void move(const glm::vec3& delta);
    
protected:
    virtual glm::mat4 onViewComputed() const override;

private:
    glm::vec3 m_WorldPosition = {0.f, 0.f, 0.f};
    glm::vec3 m_Rotation = {0.f, 0.f, 0.f};
};

} // namespace vrm