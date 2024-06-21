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

    LOG_INFO("Loading mesh from file: {}", filePath);
    LOG_TRACE("  Meshes count: {}", loader.LoadedMeshes.size());
    for (const auto& mesh : loader.LoadedMeshes)
    {
    LOG_TRACE("    Mesh: {}", mesh.MeshName);
    LOG_TRACE("      Vertices count: {}", mesh.Vertices.size());
    LOG_TRACE("      Indices count: {}", mesh.Indices.size());
    LOG_TRACE("      Material: {}", mesh.MeshMaterial.name);
    }
    LOG_TRACE("  Vertices count: {}", loader.LoadedVertices.size());
    LOG_TRACE("  Indices count: {}", loader.LoadedIndices.size());
    LOG_TRACE("  Materials count: {}", loader.LoadedMaterials.size());

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


    for (const auto& material : loader.LoadedMaterials)
    {
        LOG_TRACE("  Material: {}", material.name);
        LOG_TRACE("    Ambient:  ({}, {}, {})", material.Ka.X, material.Ka.Y, material.Ka.Z);
        LOG_TRACE("    Diffuse:  ({}, {}, {})", material.Kd.X, material.Kd.Y, material.Kd.Z);
        LOG_TRACE("    Specular: ({}, {}, {})", material.Ks.X, material.Ks.Y, material.Ks.Z);
        LOG_TRACE("    Shininess: {}", material.Ns);
    }

    m_MeshData = MeshData(std::move(vertices), std::move(indices));
    m_RenderMesh = std::make_unique<RenderMesh>(m_MeshData);

    LOG_INFO("Loaded mesh from file: {}", filePath);

    return true;
}

} // namespace vrm