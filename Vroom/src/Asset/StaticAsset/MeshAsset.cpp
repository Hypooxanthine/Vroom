#include "Vroom/Asset/StaticAsset/MeshAsset.h"

#include <OBJ_Loader/OBJ_Loader.h>

#include "Vroom/Core/Assert.h"
#include "Vroom/Asset/AssetInstance/MeshInstance.h"

namespace vrm
{

MeshAsset::MeshAsset()
    : StaticAsset(), m_RenderMesh(nullptr)
{
}

MeshAsset::~MeshAsset()
{
}

MeshInstance MeshAsset::createInstance()
{
    return MeshInstance(this);
}

const MeshData& MeshAsset::getMeshData() const
{
    return m_MeshData;
}

const RenderMesh& MeshAsset::getRenderMesh() const
{
    VRM_DEBUG_ASSERT_MSG(m_RenderMesh != nullptr, "RenderMesh is nullptr. Did you forget to load the mesh?");
    return *m_RenderMesh;
}

bool MeshAsset::loadImpl(const std::string& filePath)
{
    std::string extension = StaticAsset::getExtension(filePath);
    if (extension == "obj")
    {
        return loadObj(filePath);
    }
    
    if (extension == "")
        LOG_ERROR("Failed to get extension from file path: {}", filePath);
    else
        LOG_ERROR("Unsupported file extension: {}", extension);
    
    return false;
}

bool MeshAsset::loadObj(const std::string& filePath)
{
    objl::Loader loader;
    if (!loader.LoadFile(filePath))
    {
        LOG_ERROR("Failed to load obj file: {}", filePath);
        return false;
    }

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    vertices.reserve(loader.LoadedVertices.size());
    indices.reserve(loader.LoadedIndices.size());

    for (const auto& vertex : loader.LoadedVertices)
    {
        vertices.emplace_back(
            Vertex{ 
                { vertex.Position.X         , vertex.Position.Y         , vertex.Position.Z },
                { vertex.Normal.X           , vertex.Normal.Y           , vertex.Normal.Z },
                { vertex.TextureCoordinate.X, vertex.TextureCoordinate.Y }
            }
        );
    }

    for (const auto& index : loader.LoadedIndices)
    {
        indices.emplace_back(index);
    }

    m_MeshData = MeshData(std::move(vertices), std::move(indices));
    LOG_INFO("Mesh vertices: {}, triangles: {}", m_MeshData.getVertexCount(), m_MeshData.getTriangleCount());
    m_RenderMesh = std::make_unique<RenderMesh>(m_MeshData);

    return true;
}

} // namespace vrm