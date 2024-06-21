#pragma once

#include <glm/glm.hpp>

namespace vrm
{

class CameraBasic
{
public:
    CameraBasic() = default;
    CameraBasic(float near, float far);
    virtual ~CameraBasic() = default;

    void setNear(float near);
    void setFar(float far);

    inline float getNear() const { return m_Near; }
    inline float getFar() const { return m_Far; }

    virtual void setViewportSize(float width, float height) {}

    const glm::mat4& getView() const;
    const glm::mat4& getProjection() const;
    const glm::mat4& getViewProjection() const;

    glm::vec3 getForwardVector() const;
    glm::vec3 getUpVector() const;
    glm::vec3 getRightVector() const;

protected:
    void markViewDirty() { m_ViewDirty = true; m_ViewProjectionDirty = true; }
    void markProjectionDirty() { m_ProjectionDirty = true; m_ViewProjectionDirty = true;}

    virtual glm::mat4 onViewComputed() const = 0;
    virtual glm::mat4 onProjectionComputed() const = 0;

private:
    void computeView() const;
    void computeProjection() const;
    void computeViewProjection() const;

private:
    float m_Near = 0.f, m_Far = 0.f;

    mutable glm::mat4 m_View,
        m_Projection,
        m_ViewProjection;
    mutable bool m_ViewDirty = true,
        m_ProjectionDirty = true,
        m_ViewProjectionDirty = true;
};

} // namespace vrm