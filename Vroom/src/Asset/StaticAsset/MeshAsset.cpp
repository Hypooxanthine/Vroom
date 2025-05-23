#include "Vroom/Asset/StaticAsset/MeshAsset.h"

#include <OBJ_Loader/OBJ_Loader.h>

#include "Vroom/Core/Assert.h"

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/StaticAsset/MaterialAsset.h"

using namespace vrm;

MeshAsset::SubMesh::SubMesh(RenderMesh&& render, MeshData&& data, MaterialAsset::Handle instance)
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

void MeshAsset::addSubmesh(const MeshData& mesh, MaterialAsset::Handle instance)
{
  m_SubMeshes.emplace_back(SubMesh(RenderMesh(mesh), MeshData(mesh), instance));
}

void MeshAsset::addSubmesh(const MeshData& mesh)
{
  MaterialAsset::Handle materialInstance = AssetManager::Get().getAsset<MaterialAsset>("Resources/Engine/Material/Mat_Default.asset");
  m_SubMeshes.emplace_back(SubMesh(RenderMesh(mesh), MeshData(mesh), materialInstance));
}

void MeshAsset::clear()
{
  m_SubMeshes.clear();
}

bool MeshAsset::loadImpl(const std::string& filePath)
{
  VRM_LOG_TRACE("Loading mesh: {}", filePath);
  
  std::string extension = StaticAsset::getExtension(filePath);
  if (extension == "obj")
  {
    return loadObj(filePath);
  }

  if (extension == "")
    VRM_LOG_ERROR("Failed to get extension from file path: {}", filePath);
  else
    VRM_LOG_ERROR("Unsupported file extension: {}", extension);

  return false;
}

bool MeshAsset::loadObj(const std::string& filePath)
{
  objl::Loader loader;
  if (!loader.LoadFile(filePath))
  {
    VRM_LOG_ERROR("Failed to load obj file: {}", filePath);
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

  // VRM_LOG_TRACE("| Loading {} submeshes.", loader.LoadedMeshes.size());

  for (const auto& mesh : loader.LoadedMeshes)
  {
    // VRM_LOG_TRACE("| | SubMesh: {}", mesh.MeshName);
    // VRM_LOG_TRACE("| | | Vertices count: {}", mesh.Vertices.size());
    // VRM_LOG_TRACE("| | | Indices count: {}", mesh.Indices.size());
    // VRM_LOG_TRACE("| | | Material: {}", mesh.MaterialName);

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

    MaterialAsset::Handle materialInstance;

    if (!mesh.MaterialName.empty())
    {
      if (AssetManager::Get().tryLoadAsset<MaterialAsset>(mesh.MaterialName))
        materialInstance = AssetManager::Get().getAsset<MaterialAsset>(mesh.MaterialName);
      else
        VRM_LOG_WARN("Couldn't load material \"{}\", falling back to default material.", mesh.MaterialName);
    }

    if (!materialInstance.isValid()) // Fall back to default material
    {
      materialInstance = AssetManager::Get().getAsset<MaterialAsset>("Resources/Engine/Material/DefaultMaterial.json");
    }

    MeshData meshData(std::move(vertices), std::move(indices));
    RenderMesh renderMesh(meshData);

    m_SubMeshes.emplace_back(std::move(renderMesh), std::move(meshData), materialInstance);

    // VRM_LOG_TRACE("| | Loaded sub mesh: {}", mesh.MeshName);
  }

  // VRM_LOG_TRACE("| Submeshes loaded.");

  // VRM_LOG_INFO("Mesh loaded.");

  return true;
}
