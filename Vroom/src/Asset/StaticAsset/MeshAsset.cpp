#include "Vroom/Asset/StaticAsset/MeshAsset.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
  {
    VRM_LOG_ERROR("Failed to load mesh: {}. Error: {}", filePath, importer.GetErrorString());
    return false;
  }

  _processNode(scene->mRootNode, scene);

  // m_SubMeshes.clear();
  // return loadObj(filePath);

  return true;
}

void MeshAsset::_processNode(aiNode* node, const aiScene* scene)
{
  for (unsigned int i = 0; i < node->mNumMeshes; i++)
  {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    _processMesh(mesh, scene);
  }

  for (unsigned int i = 0; i < node->mNumChildren; i++)
  {
    _processNode(node->mChildren[i], scene);
  }
}

void MeshAsset::_processMesh(aiMesh* mesh, const aiScene* scene)
{
  std::vector<Vertex> vertices;
  vertices.reserve(mesh->mNumVertices);

  for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
  {
    Vertex& v = vertices.emplace_back();

    const aiVector3D& aPosition = mesh->mVertices[i];
    v.position = glm::vec3{ aPosition.x, aPosition.y, aPosition.z };

    const aiVector3D& aNormal = mesh->mNormals[i];
    v.normal = glm::vec3{ aNormal.x, aNormal.y, aNormal.z };
    
    if (mesh->mTextureCoords[0])
    {
      const aiVector3D& aTexCoords = mesh->mTextureCoords[0][i];
      v.texCoords = glm::vec2{ aTexCoords.x, aTexCoords.y };
    }
  }

  std::vector<unsigned int> indices;
  indices.reserve(mesh->mNumFaces * 3);

  for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
  {
    const aiFace& f = mesh->mFaces[i];
    for (unsigned int j = 0; j < f.mNumIndices; ++j)
    {
      indices.emplace_back(f.mIndices[j]);
    }
  }
  
  MeshData meshData(std::move(vertices), std::move(indices));
  RenderMesh renderMesh(meshData);
  std::string matName = scene->mMaterials[mesh->mMaterialIndex]->GetName().C_Str();
  if (matName == AI_DEFAULT_MATERIAL_NAME)
  {
    matName = "Resources/Engine/Material/DefaultMaterial.json";
  }

  MaterialAsset::Handle mat;
  if (AssetManager::Get().tryLoadAsset<MaterialAsset>(matName))
    mat = AssetManager::Get().getAsset<MaterialAsset>(matName);
  else
  {
    VRM_LOG_WARN("Couldn't load material \"{}\", falling back to default material.", matName);
    mat = AssetManager::Get().getAsset<MaterialAsset>("Resources/Engine/Material/DefaultMaterial.json");
  }
  m_SubMeshes.emplace_back(std::move(renderMesh), std::move(meshData), mat);
}
