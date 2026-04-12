#pragma once

#include "RenderObjects/MeshData.h"

#include "Rasterizer/Buffer.h"
#include "Rasterizer/VertexArray.h"
#include "RenderObjects/Api.h"

namespace vrm::render
{

  class VRM_RENDEROBJECTS_API Mesh
  {
  public:
    Mesh(const MeshData& meshData);

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    Mesh(Mesh&&);
    Mesh& operator=(Mesh&&);

    ~Mesh();

    const gl::VertexArray& getVertexArray() const { return m_vertexArray; }
    const gl::Buffer& getIndexBuffer() const { return m_indexBuffer; }

    inline GLuint getIndexCount() const { return m_indexCount; }

  private:
    gl::Buffer m_vertexBuffer, m_indexBuffer;
    gl::VertexArray m_vertexArray;

    GLuint m_indexCount;
  };

} // namespace vrm
