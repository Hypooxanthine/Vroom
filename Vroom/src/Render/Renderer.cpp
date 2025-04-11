#include "Vroom/Render/Renderer.h"

#include <array>
#include <glm/gtc/matrix_transform.hpp>

#include "Vroom/Core/Application.h"

#include "Vroom/Render/Abstraction/GLCall.h"
#include "Vroom/Render/Abstraction/VertexArray.h"
#include "Vroom/Render/Abstraction/VertexBuffer.h"
#include "Vroom/Render/Abstraction/IndexBuffer.h"
#include "Vroom/Render/Abstraction/Shader.h"
#include "Vroom/Render/Abstraction/FrameBuffer.h"

#include "Vroom/Render/RawShaderData/SSBOPointLightData.h"

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/StaticAsset/ShaderAsset.h"
#include "Vroom/Asset/StaticAsset/MaterialAsset.h"
#include "Vroom/Asset/StaticAsset/TextureAsset.h"
#include "Vroom/Asset/StaticAsset/MeshAsset.h"

#include "Vroom/Scene/Components/PointLightComponent.h"
#include "Vroom/Scene/Components/MeshComponent.h"

#include "Vroom/Scene/Scene.h"

static float SCREEN_QUAD_VERTICES[] = {
    -1.f, -1.f, 0.f, 0.f,
    1.f, -1.f, 1.f, 0.f,
    1.f, 1.f, 1.f, 1.f,
    -1.f, 1.f, 0.f, 1.f};

static unsigned int SCREEN_QUAD_INDICES[] = {
    0, 1, 2,
    2, 3, 0};

using namespace vrm;
using namespace vrm::gl;

std::unique_ptr<Renderer> Renderer::s_Instance = nullptr;

Renderer::Renderer()
{
  GLCall(glEnable(GL_CULL_FACE));
  GLCall(glCullFace(GL_BACK));
  GLCall(glFrontFace(GL_CCW));
}

Renderer::~Renderer()
{
}

void Renderer::Init()
{
  VRM_ASSERT_MSG(s_Instance == nullptr, "Renderer already initialized.");
  s_Instance.reset(new Renderer());
}

void Renderer::Shutdown()
{
  s_Instance.reset();
}

Renderer &Renderer::Get()
{
  VRM_ASSERT_MSG(s_Instance != nullptr, "Renderer not initialized.");
  return *s_Instance;
}

void Renderer::beginScene(const CameraBasic &camera)
{
  m_Camera = &camera;
}

void Renderer::endScene(const FrameBuffer &target)
{
  // Setting up lights
  m_LightRegistry.update();

  // Clustered shading
  m_ClusteredLights.setupClusters({12, 12, 24}, *m_Camera);
  m_ClusteredLights.processLights(*m_Camera, m_LightRegistry.getPointLightsStorageBuffer());

  // Rendering to the requested target
  target.bind();
  target.clearColorBuffer();
  GLCall(glViewport(m_ViewportOrigin.x, m_ViewportOrigin.y, m_ViewportSize.x, m_ViewportSize.y));

  // Drawing meshes
  for (const auto &queuedMesh : m_Meshes)
  {
    drawMesh(*queuedMesh.mesh, queuedMesh.material, *queuedMesh.model);
  }

  // Clearing data for next frame
  m_Camera = nullptr;
  m_Meshes.clear();
}

void Renderer::drawMesh(const MeshComponent &mesh, const glm::mat4 &model)
{
  uint8_t matSlot = 0;
  for (const auto& submesh : mesh.getMesh()->getSubMeshes())
  {
    QueuedMesh m;
    m.mesh = &submesh.renderMesh;
    m.material = mesh.getMaterials().getMaterial(matSlot++);
    m.model = &model;
    m_Meshes.push_back(std::move(m));
  }
}

void Renderer::registerPointLight(const glm::vec3 &position, const PointLightComponent &pointLight, size_t identifier)
{
  m_LightRegistry.submitPointLight(pointLight, position, identifier);
}

void Renderer::unregisterPointLight(size_t identifier)
{
  m_LightRegistry.removePointLight(identifier);
}

void Renderer::updatePointLight(const glm::vec3& position, const PointLightComponent& pointLight, size_t identifier)
{
  m_LightRegistry.updatePointLight(pointLight, position, identifier);
}

void Renderer::drawMesh(const RenderMesh& mesh, MaterialAsset::Handle mat, const glm::mat4& model) const
{
  VRM_DEBUG_ASSERT_MSG(m_Camera, "No camera set for rendering. Did you call beginScene?");

  const auto cameraPos = m_Camera->getPosition();

  // Binding data
  mesh.getVertexArray().bind();
  mesh.getIndexBuffer().bind();

  // Material uniforms
  mat->applyUniforms();

  const Shader &shader = mat->getShader();
  shader.bind();

  // Setting uniforms
  shader.setUniformMat4f("u_Model", model);
  shader.setUniformMat4f("u_View", m_Camera->getView());
  shader.setUniformMat4f("u_Projection", m_Camera->getProjection());
  shader.setUniformMat4f("u_ViewProjection", m_Camera->getViewProjection());
  shader.setUniform3f("u_ViewPosition", cameraPos);
  shader.setUniform1f("u_Near", m_Camera->getNear());
  shader.setUniform1f("u_Far", m_Camera->getFar());
  shader.setUniform2ui("u_ViewportSize", m_ViewportSize.x, m_ViewportSize.y);
  shader.setStorageBuffer("LightBlock", m_LightRegistry.getPointLightsStorageBuffer());
  shader.setStorageBuffer("ClusterInfoBlock", m_ClusteredLights.getClustersShaderStorage());

  // Drawing data
  GLCall(glDrawElements(GL_TRIANGLES, (GLsizei)mesh.getIndexCount(), GL_UNSIGNED_INT, nullptr));
}

const glm::vec<2, unsigned int> &Renderer::getViewportOrigin() const
{
  return m_ViewportOrigin;
}

const glm::vec<2, unsigned int> &Renderer::getViewportSize() const
{
  return m_ViewportSize;
}

void Renderer::setViewport(const glm::vec<2, unsigned int> &o, const glm::vec<2, unsigned int> &s)
{
  setViewportOrigin(o);
  setViewportSize(s);
}

void Renderer::setViewportOrigin(const glm::vec<2, unsigned int> &o)
{
  m_ViewportOrigin = o;
}

void Renderer::setViewportSize(const glm::vec<2, unsigned int> &s)
{
  m_ViewportSize = s;
}
