#pragma once

#include "Vroom/Asset/AssetData/MeshData.h"

#include "Vroom/Render/Abstraction/VertexArray.h"
#include "Vroom/Render/Abstraction/IndexBuffer.h"
#include "Vroom/Render/Abstraction/VertexBuffer.h"
#include "Vroom/Render/Abstraction/VertexBufferLayout.h"

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

    const gl::VertexArray& getVertexArray() const { return m_VertexArray; }
    const gl::IndexBuffer& getIndexBuffer() const { return m_IndexBuffer; }

private:
    gl::VertexBuffer m_VertexBuffer;
    gl::IndexBuffer m_IndexBuffer;
    gl::VertexArray m_VertexArray;
    gl::VertexBufferLayout m_VertexBufferLayout;
};

} // namespace vrm
