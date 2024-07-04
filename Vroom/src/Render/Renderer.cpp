#include "Vroom/Render/Renderer.h"

#include <array>
#include <glm/gtc/matrix_transform.hpp>

#include "Vroom/Render/Abstraction/GLCall.h"
#include "Vroom/Render/Abstraction/VertexArray.h"
#include "Vroom/Render/Abstraction/VertexBuffer.h"
#include "Vroom/Render/Abstraction/VertexBufferLayout.h"
#include "Vroom/Render/Abstraction/IndexBuffer.h"
#include "Vroom/Render/Abstraction/Shader.h"

#include "Vroom/Render/RawShaderData/SSBOPointLightData.h"

#include "Vroom/Asset/StaticAsset/MeshAsset.h"
#include "Vroom/Asset/StaticAsset/MaterialAsset.h"
#include "Vroom/Asset/AssetData/MeshData.h"
#include "Vroom/Asset/AssetData/MaterialData.h"
#include "Vroom/Asset/StaticAsset/TextureAsset.h"

#include "Vroom/Scene/Components/PointLightComponent.h"

#include "Vroom/Scene/Scene.h"

namespace vrm
{

Renderer::Renderer()
{
    GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.f));
    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

Renderer::~Renderer()
{
}

void Renderer::beginScene(const Scene& scene)
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    GLCall(glViewport(m_ViewportOrigin.x, m_ViewportOrigin.y, m_ViewportSize.x, m_ViewportSize.y));

    m_Camera = &scene.getCamera();
}

void Renderer::endScene()
{
    // Setting up lights.
    setupLights();

    // Drawing meshes.
    for (const auto& mesh : m_Meshes)
    {
        drawMesh(mesh.mesh, mesh.model);
    }

    // Clearing data for next frame.
    m_Camera = nullptr;
    m_Meshes.clear();
    m_PointLights.clear();
}

void Renderer::submitMesh(const MeshInstance& mesh, const glm::mat4& model)
{
    m_Meshes.push_back({ mesh, model });
}

void Renderer::submitPointLight(const glm::vec3& position, const PointLightComponent& pointLight)
{
    m_PointLights.push_back({ position, pointLight });
}

void Renderer::drawMesh(const MeshInstance& mesh, const glm::mat4& model) const
{
    VRM_DEBUG_ASSERT_MSG(m_Camera, "No camera set for rendering. Did you call beginScene?");

    const auto& subMeshes = mesh.getStaticAsset()->getSubMeshes();

    const auto cameraPos = m_Camera->getPosition();

    for (const auto& subMesh : subMeshes)
    {
        // Binding data
        subMesh.renderMesh.getVertexArray().bind();
        subMesh.renderMesh.getIndexBuffer().bind();

        const Shader& shader = subMesh.materialInstance.getStaticAsset()->getShader();
        shader.bind();

        // Setting uniforms
        shader.setUniformMat4f("u_Model", model);
        shader.setUniformMat4f("u_View", m_Camera->getView());
        shader.setUniformMat4f("u_Projection", m_Camera->getProjection());
        shader.setUniformMat4f("u_ViewProjection", m_Camera->getViewProjection());
        shader.setUniform3f("u_ViewPosition", cameraPos);

        // Setting material textures uniforms
        size_t textureCount = subMesh.materialInstance.getStaticAsset()->getTextureCount();
        if (textureCount > 0)
        {
            std::vector<int> textureSlots(textureCount);
            for (size_t i = 0; i < textureCount; ++i)
            {
                const auto& texture = subMesh.materialInstance.getStaticAsset()->getTexture(i);
                texture.getStaticAsset()->getGPUTexture().bind((unsigned int)i);
                textureSlots[i] = (int)i;
            }

            shader.setUniform1iv("u_Texture", (int)textureCount, textureSlots.data());
        }

        // Drawing data
        GLCall(glDrawElements(GL_TRIANGLES, (GLsizei)subMesh.renderMesh.getIndexBuffer().getCount(), GL_UNSIGNED_INT, nullptr));
    }

}

const glm::vec<2, unsigned int>& Renderer::getViewportOrigin() const
{
    return m_ViewportOrigin;
}

const glm::vec<2, unsigned int>& Renderer::getViewportSize() const
{
    return m_ViewportSize;
}

void Renderer::setViewport(const glm::vec<2, unsigned int>& o, const glm::vec<2, unsigned int>& s)
{
    setViewportOrigin(o);
    setViewportSize(s);
}

void Renderer::setViewportOrigin(const glm::vec<2, unsigned int>& o)
{
    m_ViewportOrigin = o;
}

void Renderer::setViewportSize(const glm::vec<2, unsigned int>& s)
{
    m_ViewportSize = s;
}

void Renderer::setupLights()
{
    std::vector<SSBOPointLightData> pointLightsData(m_PointLights.size());
    for (size_t i = 0; i < m_PointLights.size(); ++i)
    {
        const auto& pointLight = m_PointLights[i];
        pointLightsData[i].position = pointLight.position;
        pointLightsData[i].color = pointLight.pointLight.color;
        pointLightsData[i].intensity = pointLight.pointLight.intensity;
        pointLightsData[i].radius = pointLight.pointLight.radius;
    }

    int pointLightCount = static_cast<int>(m_PointLights.size());

    // SSBO for shaders
    m_PointLightsSSBO.bind();
    m_PointLightsSSBO.setData(nullptr, pointLightsData.size() * sizeof(SSBOPointLightData) + sizeof(int));
    m_PointLightsSSBO.setSubData(&pointLightCount, sizeof(int), 0);
    m_PointLightsSSBO.setSubData(pointLightsData.data(), pointLightsData.size() * sizeof(SSBOPointLightData), sizeof(int));
    m_PointLightsSSBO.setBindingPoint(0);
    m_PointLightsSSBO.unbind();
}

} // namespace vrm
