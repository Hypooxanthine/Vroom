#include "Vroom/Render/Camera/PerspectiveCameraBasic.h"

#include <glm/ext/matrix_clip_space.hpp>

namespace vrm
{

PerspectiveCameraBasic::PerspectiveCameraBasic(float near, float far, float fov, float aspectRatio)
    : CameraBasic(near, far), m_Fov(fov), m_AspectRatio(aspectRatio)
{
    markProjectionDirty();
}

void PerspectiveCameraBasic::setFov(float fov)
{
    m_Fov = fov;
    markProjectionDirty();
}

void PerspectiveCameraBasic::setAspectRatio(float ratio)
{
    m_AspectRatio = ratio;
    markProjectionDirty();
}

void PerspectiveCameraBasic::setViewportSize(float width, float height)
{
    setAspectRatio(width / height);
}

glm::mat4 PerspectiveCameraBasic::onProjectionComputed() const
{
    return glm::perspective(m_Fov, m_AspectRatio, getNear(), getFar());
}

} // namespace vrm