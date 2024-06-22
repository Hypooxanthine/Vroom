#include "Vroom/Render/Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Vroom/Render/Abstraction/GLCall.h"
#include "Vroom/Asset/AssetInstance/MeshInstance.h"
#include "Vroom/Asset/StaticAsset/MeshAsset.h"
#include "Vroom/Asset/StaticAsset/MaterialAsset.h"
#include "Vroom/Asset/StaticAsset/ShaderAsset.h"
#include "Vroom/Asset/AssetData/MeshData.h"
#include "Vroom/Asset/AssetData/MaterialData.h"

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

void Renderer::beginScene()
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    GLCall(glViewport(m_ViewportOrigin.x, m_ViewportOrigin.y, m_ViewportSize.x, m_ViewportSize.y));
}

void Renderer::endScene()
{
    
}

void Renderer::drawTriangles(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    // Binding data
    va.bind();
    ib.bind();
    shader.bind();

    // Drawing data
    GLCall(glDrawElements(GL_TRIANGLES, (GLsizei)ib.getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::drawPoints(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    // Binding data
    va.bind();
    ib.bind();
    shader.bind();

    // Drawing data
    GLCall(glDrawElements(GL_POINTS, (GLsizei)ib.getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::drawMesh(const MeshInstance& mesh, const CameraBasic& camera, const glm::mat4& model) const
{
    const auto& subMeshes = mesh.getStaticAsset()->getSubMeshes();

    for (const auto& subMesh : subMeshes)
    {
        // Binding data
        subMesh.renderMesh.getVertexArray().bind();
        subMesh.renderMesh.getIndexBuffer().bind();

        const Shader& shader = subMesh.materialInstance.getStaticAsset()->getShaderInstance().getStaticAsset()->getShader();
        shader.bind();

        // Setting uniforms
        shader.setUniformMat4f("u_Model", model);
        shader.setUniformMat4f("u_View", camera.getView());
        shader.setUniformMat4f("u_Projection", camera.getProjection());
        shader.setUniformMat4f("u_ViewProjection", camera.getViewProjection());

        subMesh.materialInstance.getStaticAsset()->getMaterialData().applyUniforms(shader);

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


