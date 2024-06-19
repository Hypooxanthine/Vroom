#include "Vroom/Asset/AssetData/MeshData.h"

namespace vrm
{

MeshData::MeshData(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
    : m_Vertices(std::move(vertices)), m_Indices(std::move(indices))
{
}

MeshData::MeshData()
    : m_Vertices(), m_Indices()
{
}

MeshData::MeshData(const MeshData& other)
    : m_Vertices(other.m_Vertices), m_Indices(other.m_Indices)
{
}

MeshData::MeshData(MeshData&& other)
    : m_Vertices(std::move(other.m_Vertices)), m_Indices(std::move(other.m_Indices))
{
}

MeshData& MeshData::operator=(const MeshData& other)
{
    if (this != &other)
    {
        m_Vertices = other.m_Vertices;
        m_Indices = other.m_Indices;
    }

    return *this;
}

MeshData& MeshData::operator=(MeshData&& other)
{
    if (this != &other)
    {
        m_Vertices = std::move(other.m_Vertices);
        m_Indices = std::move(other.m_Indices);
    }

    return *this;
}

MeshData::~MeshData()
{
}



} // namespace vrm
