#pragma once

#include "Vroom/Api.h"
#include "Vroom/Render/Camera/CameraBasic.h"

namespace vrm
{

class VRM_API PerspectiveCameraBasic : public CameraBasic
{
public:
    PerspectiveCameraBasic() = default;
    PerspectiveCameraBasic(float near, float far, float fov, float aspectRatio);

    void setFov(float fov);
    void setAspectRatio(float ratio);

    inline float getFov() const { return m_Fov; }
    inline float getAspectRatio() const { return m_AspectRatio; }

    void setViewportSize(float width, float height) override;

private:
    glm::mat4 onProjectionComputed() const override; // This is now private and no longer virtual.

private:
    // Projection
    float m_Fov = 0.f, m_AspectRatio = 0.f;

};

} // namespace vrm