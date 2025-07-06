#include "Vroom/Render/RenderObject/RenderMesh.h"

#include "Vroom/Core/Log.h"

using namespace vrm;

RenderMesh::RenderMesh(const MeshData &meshData)
{
  gl::Buffer::Desc desc;

  {
    desc.capacity = meshData.getVertexCount() * sizeof(Vertex);
    m_vertexBuffer.create(desc, std::span{ meshData.getVertices() });
  }

  {
    desc.capacity = meshData.getIndexCount() * sizeof(uint32_t);
    m_indexBuffer.create(desc, std::span{ meshData.getIndices() });
  }

  m_indexCount = static_cast<GLuint>(meshData.getIndexCount());

  m_vertexArray.create();
  gl::VertexArray::Attribute attribute;
  uint32_t attribOrderer = 0;

  {
    attribute.componentCount = 3;
    attribute.componentType = GL_FLOAT;
    attribute.normalized = false;
    attribute.vertexBufferOffsetBytes = offsetof(Vertex, position);
    attribute.vertexBufferStrideBytes = sizeof(Vertex);

    m_vertexArray.enableAttribute(attribOrderer++, attribute, m_vertexBuffer);
  }

  {
    attribute.componentCount = 3;
    attribute.componentType = GL_FLOAT;
    attribute.normalized = false;
    attribute.vertexBufferOffsetBytes = offsetof(Vertex, normal);
    attribute.vertexBufferStrideBytes = sizeof(Vertex);

    m_vertexArray.enableAttribute(attribOrderer++, attribute, m_vertexBuffer);
  }

  {
    attribute.componentCount = 2;
    attribute.componentType = GL_FLOAT;
    attribute.normalized = false;
    attribute.vertexBufferOffsetBytes = offsetof(Vertex, texCoords);
    attribute.vertexBufferStrideBytes = sizeof(Vertex);

    m_vertexArray.enableAttribute(attribOrderer++, attribute, m_vertexBuffer);
  }
}

RenderMesh::RenderMesh(RenderMesh &&other)
{
  *this = std::move(other);
}

RenderMesh &RenderMesh::operator=(RenderMesh &&other)
{
  if (this != &other)
  {
    m_vertexBuffer = std::move(other.m_vertexBuffer);
    m_indexBuffer = std::move(other.m_indexBuffer);
    m_vertexArray = std::move(other.m_vertexArray);
    
    m_indexCount = other.m_indexCount;
    other.m_indexCount = 0;
  }

  return *this;
}

RenderMesh::~RenderMesh()
{
}
