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

    const VertexArray& getVertexArray() const { return m_VertexArray; }
    const IndexBuffer& getIndexBuffer() const { return m_IndexBuffer; }

private:
    VertexBuffer m_VertexBuffer;
    IndexBuffer m_IndexBuffer;
    VertexArray m_VertexArray;
    VertexBufferLayout m_VertexBufferLayout;
};

} // namespace vrm
