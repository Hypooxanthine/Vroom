#include "Vroom/Render/RenderObject/RenderMesh.h"

#include "Vroom/Core/Log.h"

using namespace vrm;

RenderMesh::RenderMesh(const MeshData &meshData)
{
  m_VertexBuffer.setData(meshData.getVertices());
  m_IndexBuffer.setData(meshData.getIndices());

  // m_VertexBufferLayout.pushFloat(3);
  // m_VertexBufferLayout.pushFloat(3);
  // m_VertexBufferLayout.pushFloat(2);

  // m_VertexArray.addBuffer(m_VertexBuffer, m_VertexBufferLayout);
  m_VertexArray.create();
  m_VertexArray.defineAttribute<0>(m_VertexBuffer);
  m_VertexArray.defineAttribute<1>(m_VertexBuffer);
  m_VertexArray.defineAttribute<2>(m_VertexBuffer);
}

RenderMesh::RenderMesh(RenderMesh &&other)
    : m_VertexBuffer(std::move(other.m_VertexBuffer)),
      m_IndexBuffer(std::move(other.m_IndexBuffer)),
      m_VertexArray(std::move(other.m_VertexArray))
{
}

RenderMesh &RenderMesh::operator=(RenderMesh &&other)
{
  if (this != &other)
  {
    m_VertexBuffer = std::move(other.m_VertexBuffer);
    m_IndexBuffer = std::move(other.m_IndexBuffer);
    m_VertexArray = std::move(other.m_VertexArray);
  }

  return *this;
}

RenderMesh::~RenderMesh()
{
}
