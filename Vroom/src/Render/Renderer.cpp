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

    m_LightRegistry.setBindingPoint(0);
    m_ClusteredLights.setBindingPoints(2, 1);
}

Renderer::~Renderer()
{
}

void Renderer::beginScene(const Scene& scene)
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    GLCall(glViewport(m_ViewportOrigin.x, m_ViewportOrigin.y, m_ViewportSize.x, m_ViewportSize.y));

    m_Camera = &scene.getCamera();

    m_LightRegistry.beginFrame();
}

void Renderer::endScene()
{
    // Setting up lights.
    m_LightRegistry.endFrame();

    const auto& pointLights = m_LightRegistry.getPointLights();
    
    //for (const auto& [index, pointLight] : pointLights)
    //    LOG_TRACE("Point light index: {}", index);

    // Setting up clusters.
    m_ClusteredLights.beginFrame({ 32, 32, 32 }, *m_Camera);

    for (const auto& [index, pointLight] : pointLights)
    {
        m_ClusteredLights.submitLight(pointLight.position, pointLight.radius, index);
    }

    m_ClusteredLights.endFrame();

    // Drawing meshes.
    for (const auto& mesh : m_Meshes)
    {
        drawMesh(mesh.mesh, mesh.model);
    }

    // Clearing data for next frame.
    m_Camera = nullptr;
    m_Meshes.clear();
}

void Renderer::submitMesh(const MeshInstance& mesh, const glm::mat4& model)
{
    m_Meshes.push_back({ mesh, model });
}

void Renderer::submitPointLight(const glm::vec3& position, const PointLightComponent& pointLight, const std::string& identifier)
{
    m_LightRegistry.submitPointLight(pointLight, position, identifier);
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
        shader.setUniform1f("u_Near", m_Camera->getNear());
        shader.setUniform1f("u_Far", m_Camera->getFar());

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

} // namespace vrm
