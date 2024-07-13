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

    /**
     * @brief Get the position of the camera in world space.
     * 
     * @return glm::vec3 The position of the camera.
     */
    inline glm::vec3 getPosition() const override { return m_WorldPosition; }

    /**
     * @brief Get the rotation of the camera in world space.
     * 
     * @return glm::vec3 The rotation of the camera (in degrees).
     */
    inline const glm::vec3& getRotation() const { return m_Rotation; }

    /* Setters */

    /**
     * @brief Set the position of the camera in world space.
     * 
     * @param pos The new position of the camera.
     */
    void setWorldPosition(const glm::vec3& pos);

    /**
     * @brief Set the rotation of the camera in world space.
     * 
     * @param rot The new rotation of the camera (in degrees).
     */
    void setRotation(const glm::vec3& rot);

    /* Modifiers */

    /**
     * @brief Add a yaw rotation to the camera.
     * 
     * @param yaw The yaw rotation to add (in degrees).
     */
    void addYaw(float yaw);

    /**
     * @brief Add a pitch rotation to the camera.
     * 
     * @param pitch The pitch rotation to add (in degrees).
     */
    void addPitch(float pitch);

    /**
     * @brief Add a roll rotation to the camera.
     * 
     * @param roll The roll rotation to add (in degrees).
     */
    void addRoll(float roll);

    /**
     * @brief Move the camera in world space.
     * 
     * @param delta The translation to apply to the camera.
     */
    void move(const glm::vec3& delta);
    
protected:
    virtual glm::mat4 onViewComputed() const override;

private:
    glm::vec3 m_WorldPosition = {0.f, 0.f, 0.f};
    // In degrees
    glm::vec3 m_Rotation = {0.f, 0.f, 0.f};
};

} // namespace vrm