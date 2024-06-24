#include "Vroom/Asset/StaticAsset/MeshAsset.h"

#include <OBJ_Loader/OBJ_Loader.h>

#include "Vroom/Core/Assert.h"
#include "Vroom/Asset/AssetInstance/MeshInstance.h"

#include "Vroom/Asset/AssetManager.h"

namespace vrm
{

MeshAsset::SubMesh::SubMesh(RenderMesh&& render, MeshData&& data, MaterialInstance instance)
    : renderMesh(std::move(render)), meshData(std::move(data)), materialInstance(instance)
{
}

MeshAsset::MeshAsset()
    : StaticAsset()
{
}

MeshAsset::~MeshAsset()
{
}

MeshInstance MeshAsset::createInstance()
{
    return MeshInstance(this);
}

bool MeshAsset::loadImpl(const std::string& filePath, AssetManager& manager)
{
    std::string extension = StaticAsset::getExtension(filePath);
    if (extension == "obj")
    {
        return loadObj(filePath, manager);
    }
    
    if (extension == "")
        LOG_ERROR("Failed to get extension from file path: {}", filePath);
    else
        LOG_ERROR("Unsupported file extension: {}", extension);
    
    return false;
}

bool MeshAsset::loadObj(const std::string& filePath, AssetManager& manager)
{
    objl::Loader loader;
    if (!loader.LoadFile(filePath))
    {
        LOG_ERROR("Failed to load obj file: {}", filePath);
        return false;
    }

    std::string fileDirectoryPath;
    size_t lastSlashIndex = filePath.find_last_of('/');
    if (lastSlashIndex != std::string::npos)
    {
        fileDirectoryPath = filePath.substr(0, lastSlashIndex + 1);
    }
    else
    {
        lastSlashIndex = filePath.find_last_of('\\');
        if (lastSlashIndex != std::string::npos)
        {
            fileDirectoryPath = filePath.substr(0, lastSlashIndex + 1);
        }
    }

    LOG_INFO("Loading mesh from file: {}", filePath);
    LOG_TRACE("| Loading {} submeshes.", loader.LoadedMeshes.size());

    for (const auto& mesh : loader.LoadedMeshes)
    {
        LOG_TRACE("| | SubMesh: {}", mesh.MeshName);
        LOG_TRACE("| | | Vertices count: {}", mesh.Vertices.size());
        LOG_TRACE("| | | Indices count: {}", mesh.Indices.size());
        LOG_TRACE("| | | Material: {}", mesh.MeshMaterial.name);

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        vertices.reserve(mesh.Vertices.size());
        indices.reserve(mesh.Indices.size());

        for (const auto& vertex : mesh.Vertices)
        {
            vertices.emplace_back(
                Vertex{ 
                    { vertex.Position.X         , vertex.Position.Y         , vertex.Position.Z },
                    { vertex.Normal.X           , vertex.Normal.Y           , vertex.Normal.Z },
                    { vertex.TextureCoordinate.X, vertex.TextureCoordinate.Y }
                }
            );
        }

        for (const auto& index : mesh.Indices)
        {
            indices.emplace_back(index);
        }
        
        if (!mesh.MeshMaterial.name.empty())
        {
            MaterialInstance materialInstance = manager.getAsset<MaterialAsset>(fileDirectoryPath + mesh.MeshMaterial.name + ".asset");
            MeshData meshData(std::move(vertices), std::move(indices));
            RenderMesh renderMesh(meshData);

            m_SubMeshes.emplace_back(std::move(renderMesh), std::move(meshData), materialInstance);
        }
        else
        {
            MaterialInstance materialInstance = manager.getAsset<MaterialAsset>("Resources/Engine/Material/Mat_Default.asset");
            MeshData meshData(std::move(vertices), std::move(indices));
            RenderMesh renderMesh(meshData);

            m_SubMeshes.emplace_back(std::move(renderMesh), std::move(meshData), materialInstance);
        }

        LOG_TRACE("| | Loaded sub mesh: {}", mesh.MeshName);
    }

    LOG_TRACE("| Submeshes loaded.");

    LOG_INFO("Mesh loaded.");

    return true;
}

} // namespace vrm