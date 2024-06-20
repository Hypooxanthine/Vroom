#pragma once

#include "Vroom/Render/Camera/PerspectiveCameraBasic.h"

namespace vrm
{

class FirstPersonCamera : public PerspectiveCameraBasic
{
public:
    FirstPersonCamera() = default;
    FirstPersonCamera(const glm::vec3& position, const glm::vec3& rotation);
    FirstPersonCamera(float near, float far, float fov, float aspectRatio, const glm::vec3& position, const glm::vec3& rotation);
    virtual ~FirstPersonCamera() = default;

    /* GETTERS */

    inline const glm::vec3& getWorldPosition() const { return m_WorldPosition; }
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