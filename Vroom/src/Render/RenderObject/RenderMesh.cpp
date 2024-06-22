#include "Vroom/Render/RenderObject/RenderMesh.h"

#include "Vroom/Core/Log.h"

namespace vrm
{

RenderMesh::RenderMesh(const MeshData& meshData)
    : m_VertexBuffer(meshData.getRawVericesData(), (unsigned int)meshData.getVertexCount() * sizeof(Vertex)),
      m_IndexBuffer(meshData.getRawIndicesData(), (unsigned int)meshData.getIndexCount())
{
    m_VertexBufferLayout.pushFloat(3);
    m_VertexBufferLayout.pushFloat(3);
    m_VertexBufferLayout.pushFloat(2);

    m_VertexArray.addBuffer(m_VertexBuffer, m_VertexBufferLayout);

    LOG_INFO("RenderMesh created. Vertex count: {}, Index count: {}", meshData.getVertexCount(), meshData.getIndexCount());
}

RenderMesh::RenderMesh(RenderMesh&& other)
    : m_VertexBuffer(std::move(other.m_VertexBuffer)),
      m_IndexBuffer(std::move(other.m_IndexBuffer)),
      m_VertexArray(std::move(other.m_VertexArray)),
      m_VertexBufferLayout(std::move(other.m_VertexBufferLayout))
{
    LOG_INFO("RenderMesh moved.");
}

RenderMesh& RenderMesh::operator=(RenderMesh&& other)
{
    if (this != &other)
    {
        m_VertexBuffer = std::move(other.m_VertexBuffer);
        m_IndexBuffer = std::move(other.m_IndexBuffer);
        m_VertexArray = std::move(other.m_VertexArray);
        m_VertexBufferLayout = std::move(other.m_VertexBufferLayout);
    }

    LOG_INFO("RenderMesh moved.");
    return *this;
}

RenderMesh::~RenderMesh()
{
    LOG_INFO("RenderMesh destroyed.");
}

} // namespace vrm