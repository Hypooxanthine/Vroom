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
    VertexArray m_VertexArray;
    IndexBuffer m_IndexBuffer;
    VertexBuffer m_VertexBuffer;
    VertexBufferLayout m_VertexBufferLayout;
};

} // namespace vrm
