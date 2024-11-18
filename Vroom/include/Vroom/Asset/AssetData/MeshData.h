#pragma once

#include <vector>

#include "Vroom/Render/Abstraction/Vertex.h"

namespace vrm
{

class MeshData
{
public:
    MeshData(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
    MeshData(std::vector<Vertex>&& vertices, std::vector<uint32_t>&& indices);

    MeshData();
    MeshData(const MeshData& other);
    MeshData(MeshData&& other);
    MeshData& operator=(const MeshData& other);
    MeshData& operator=(MeshData&& other);
    ~MeshData();

    const Vertex* getRawVericesData() const { return m_Vertices.data(); }
    const uint32_t* getRawIndicesData() const { return m_Indices.data(); }

    const std::vector<Vertex>& getVertices() const { return m_Vertices; }
    const std::vector<uint32_t>& getIndices() const { return m_Indices; }

    size_t getIndexCount() const { return m_Indices.size(); }
    size_t getTriangleCount() const { return getIndexCount() / 3; }
    size_t getVertexCount() const { return m_Vertices.size(); }

private:
    std::vector<Vertex> m_Vertices;
    std::vector<uint32_t> m_Indices;
};

} // namespace vrm
