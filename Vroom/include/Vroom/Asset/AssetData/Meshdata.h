#pragma once

#include <vector>

#include "Vroom/Asset/AssetData/Vertex.h"

namespace vrm
{

class MeshData
{
public:
    MeshData(std::vector<Vertex> vertices, std::vector<uint32_t> indices);

    MeshData();
    MeshData(const MeshData& other);
    MeshData(MeshData&& other);
    MeshData& operator=(const MeshData& other);
    MeshData& operator=(MeshData&& other);
    ~MeshData();

    const Vertex* getRawVericesData() const { return m_Vertices.data(); }
    const uint32_t* getRawIndicesData() const { return m_Indices.data(); }

private:
    std::vector<Vertex> m_Vertices;
    std::vector<uint32_t> m_Indices;
};

} // namespace vrm
