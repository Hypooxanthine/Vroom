#pragma once

#include "Vroom/Render/Camera/CameraBasic.h"

namespace vrm
{

  class OrthographicCamera : public CameraBasic
  {
  public:

    OrthographicCamera(float left, float right, float bottom, float top, float near, float far);
    OrthographicCamera(float width, float height, float near, float far);
    OrthographicCamera(const OrthographicCamera& other) = default;
    virtual ~OrthographicCamera();

    inline virtual glm::vec3 getPosition() const override { return m_worldPosition; }
    void setWorldPosition(const glm::vec3& pos);
    inline void move(const glm::vec3& delta) { setWorldPosition(getPosition() + delta); };

    inline const glm::vec3& getViewDir() const { return m_viewDir; }
    void setViewDir(const glm::vec3& viewDir);

    virtual void setViewportSize(float width, float height) override;

  protected:
    virtual glm::mat4 onViewComputed() const override;
    virtual glm::mat4 onProjectionComputed() const override;

  private:

    glm::vec3 m_worldPosition = { 0.f, 0.f, 0.f };
    glm::vec3 m_viewDir = { 1.f, 0.f, 0.f };

    float m_left, m_right, m_bottom, m_top;
  };

} // namespace vrm
