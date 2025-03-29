#include "Vroom/Render/RenderObject/RenderMesh.h"

#include "Vroom/Core/Log.h"

using namespace vrm;

RenderMesh::RenderMesh(const MeshData &meshData)
{
  m_VertexBuffer.setData(meshData.getVertices());
  m_IndexBuffer.setData(meshData.getIndices());

  m_VertexBufferLayout.pushFloat(3);
  m_VertexBufferLayout.pushFloat(3);
  m_VertexBufferLayout.pushFloat(2);

  m_VertexArray.addBuffer(m_VertexBuffer, m_VertexBufferLayout);
}

RenderMesh::RenderMesh(RenderMesh &&other)
    : m_VertexBuffer(std::move(other.m_VertexBuffer)),
      m_IndexBuffer(std::move(other.m_IndexBuffer)),
      m_VertexArray(std::move(other.m_VertexArray)),
      m_VertexBufferLayout(std::move(other.m_VertexBufferLayout))
{
}

RenderMesh &RenderMesh::operator=(RenderMesh &&other)
{
  if (this != &other)
  {
    m_VertexBuffer = std::move(other.m_VertexBuffer);
    m_IndexBuffer = std::move(other.m_IndexBuffer);
    m_VertexArray = std::move(other.m_VertexArray);
    m_VertexBufferLayout = std::move(other.m_VertexBufferLayout);
  }

  return *this;
}

RenderMesh::~RenderMesh()
{
}
