#include "Vroom/Render/Passes/RenderSkyboxPass.h"

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/StaticAsset/MaterialAsset.h"
#include "Vroom/Asset/StaticAsset/MeshAsset.h"
#include "Vroom/Core/Profiling.h"
#include "Vroom/Render/Abstraction/FrameBuffer.h"
#include "Vroom/Render/Camera/CameraBasic.h"
#include "Vroom/Render/RenderObject/RenderSkybox.h"
#include "Vroom/Render/RenderView.h"

using namespace vrm;

// From LearnOpenGL
static const float s_skyboxVertices[] = {
  -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
  1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

  -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

  -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

  -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
  1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f
};

RenderSkyboxPass::RenderSkyboxPass(const std::string& name) : RenderPass(name) {}

RenderSkyboxPass::~RenderSkyboxPass() {}

void RenderSkyboxPass::onInit()
{
  VRM_CHECK_RET_MSG(skybox != nullptr, "Skybox is not valid");
  VRM_CHECK_RET_MSG(framebuffer != nullptr, "Framebuffer is not valid");

  auto matAsset = AssetManager::Get().getAsset<MaterialAsset>(
    "Resources/Engine/Material/SkyboxMaterial.json");
  VRM_CHECK_RET_MSG(matAsset.isValid(), "Could not load skybox material");

  m_material = &getPassMaterial(matAsset);
  VRM_CHECK_RET_MSG(m_material, "Could not get skybox pass material");

  auto meshAsset = AssetManager::Get().getAsset<MeshAsset>(
    "Resources/Engine/Meshes/default_cube.obj");
  VRM_CHECK_RET_MSG(meshAsset.isValid(), "Could not load skybox cube");

  gl::Buffer::Desc vboDesc;
  {
    vboDesc.capacity        = sizeof(s_skyboxVertices);
    vboDesc.allowDataUpload = false;
    vboDesc.allowMapReading = false;
    vboDesc.allowMapWriting = false;
  }
  m_vbo.create(vboDesc, std::span{ s_skyboxVertices });

  m_vao.create();

  gl::VertexArray::Attribute attr;
  {
    attr.componentCount          = 3;
    attr.componentType           = GL_FLOAT;
    attr.normalized              = false;
    attr.vertexBufferOffsetBytes = 0;
    attr.vertexBufferStrideBytes = sizeof(float) * 3;
  }

  m_vao.enableAttribute(0, attr, m_vbo);
}

void RenderSkyboxPass::onSetup(const RenderPassContext& ctx) {}

void RenderSkyboxPass::onRender(const RenderPassContext& ctx) const
{
  VRM_PROFILE_SCOPE("RenderSkyboxPass::onRender");

  if (!m_material || !skybox->getCubemap().isValid() || ctx.views.empty())
    return;

  framebuffer->bind();

  glDepthMask(GL_FALSE);
  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
  glCullFace(GL_BACK);
  glDepthFunc(GL_LEQUAL);

  gl::VertexArray::Bind(m_vao);

  const auto& shader = m_material->getShader();
  shader.bind();
  shader.setTexture("u_cubemap", skybox->getCubemap()->getGpuTexture(), 0);

  for (const render::View& view : ctx.views)
  {
    const render::Viewport& vp = view.getViewport();
    glViewport(vp.getOrigin().x, vp.getOrigin().y, vp.getSize().x,
               vp.getSize().y);

    shader.setUniformMat4f("u_view",
                           glm::mat4(glm::mat3(view.getCamera()->getView())));
    shader.setUniformMat4f("u_projection", view.getCamera()->getProjection());

    glDrawArrays(GL_TRIANGLES, 0, 36);
  }

  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LESS);
}
