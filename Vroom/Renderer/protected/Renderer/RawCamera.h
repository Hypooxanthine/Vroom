#pragma once

#include <glm/glm.hpp>
#include "Renderer/Api.h"
#include "Renderer/CameraBasic.h"

namespace vrm
{

/**
 * @brief Camera defined directly by its view and projection matrices.
 *
 */
class VRM_RENDERER_API RawCamera : public CameraBasic
{
public:

  RawCamera() = default;
  RawCamera(const glm::mat4& view, const glm::mat4& projection);
  RawCamera(const CameraBasic& cameraBasic);
  virtual ~RawCamera() = default;

  void setViewMatrix(const glm::mat4& view);
  void setProjectionMatrix(const glm::mat4& projection);

  inline float getNear() const override { return m_Near; }
  inline float getFar() const override { return m_Far; }

  inline const glm::mat4& getView() const override { return m_View; }
  inline const glm::mat4& getProjection() const override { return m_Projection; }
  inline const glm::mat4& getViewProjection() const override { return m_ViewProjection; }

private:

  void updateViewProjection() { m_ViewProjection = m_Projection * m_View; }

private:

  glm::mat4 m_View { 1.f }, m_Projection { 1.f }, m_ViewProjection { 1.f };
  float m_Near = 0.f, m_Far = 0.f;
};

} // namespace vrm
