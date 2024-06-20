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
    ~RenderMesh();

private:
    VertexBuffer m_VertexBuffer;
    IndexBuffer m_IndexBuffer;
    VertexArray m_VertexArray;
    VertexBufferLayout m_VertexBufferLayout;
};

} // namespace vrm
