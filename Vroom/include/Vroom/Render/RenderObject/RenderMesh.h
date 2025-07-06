#pragma once

#include "Vroom/Render/Abstraction/Vertex.h"
#include "Vroom/Asset/AssetData/MeshData.h"

#include "Vroom/Render/Abstraction/Buffer.h"
#include "Vroom/Render/Abstraction/VertexArray.h"

namespace vrm
{

  class RenderMesh
  {
  public:
    RenderMesh(const MeshData& meshData);

    RenderMesh(const RenderMesh&) = delete;
    RenderMesh& operator=(const RenderMesh&) = delete;

    RenderMesh(RenderMesh&&);
    RenderMesh& operator=(RenderMesh&&);

    ~RenderMesh();

    const gl::VertexArray& getVertexArray() const { return m_vertexArray; }
    const gl::Buffer& getIndexBuffer() const { return m_indexBuffer; }

    inline GLuint getIndexCount() const { return m_indexCount; }

  private:
    gl::Buffer m_vertexBuffer, m_indexBuffer;
    gl::VertexArray m_vertexArray;

    GLuint m_indexCount;
  };

} // namespace vrm
