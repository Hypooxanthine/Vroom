#include "VroomEditor/Import/ModelImporter.h"

#include <fstream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/SceneCombiner.h>
#include <assimp/postprocess.h>
#include <assimp/Exporter.hpp>

#include "Vroom/Asset/Parsing/Json.h"
#include "Vroom/Asset/AssetData/MaterialData.h"

#include "Vroom/Core/Assert.h"
#include "VroomEditor/UserInterface/AssetBrowser/AssetUtils.h"

using namespace vrm;

// ----------- Hidden implementation -------------

#define IMPL (*m_impl.get())

struct ModelImporter::Impl
{
  Impl()
  {

  }

  ~Impl()
  {

  }

  Assimp::Importer importer;

  struct ImportContext
  {
    std::filesystem::path inFile, outDir;
    const aiScene* scene = nullptr;
    std::unordered_map<std::string, json> outMaterials;
    std::unordered_set<std::filesystem::path> texturesToImport;
  } ctx;
};

// -------------------------

ModelImporter::ModelImporter()
{
  m_impl.reset(new Impl());
}

ModelImporter::~ModelImporter()
{

}

bool ModelImporter::import(const std::filesystem::path& inPath, const std::filesystem::path& outPath)
{
  const aiScene* scene = IMPL.importer.ReadFile(inPath.string(), aiProcess_Triangulate | aiProcess_GenNormals);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
  {
    VRM_LOG_ERROR("Failed to load mesh: {}. Error: {}", inPath.string(), IMPL.importer.GetErrorString());
    return false;
  }

  IMPL.ctx.inFile = inPath;
  IMPL.ctx.outDir = outPath;
  IMPL.ctx.scene = scene;

  for (unsigned int i = 0; i < IMPL.ctx.scene->mNumMaterials; ++i)
  {
    aiMaterial* mat = IMPL.ctx.scene->mMaterials[i];
    _processMaterial(mat);
  }

  _createFiles();

  IMPL.ctx = {};

  return true;
}

void ModelImporter::_createFiles()
{
  std::filesystem::path meshFileName = IMPL.ctx.inFile;
  meshFileName = meshFileName.filename();
  // Importing textures

  for (const std::filesystem::path& texPath : IMPL.ctx.texturesToImport)
  {
    std::filesystem::path src = IMPL.ctx.inFile;
    src.remove_filename();
    src = src / texPath;

    if (!std::filesystem::exists(src))
    {
      VRM_LOG_WARN("Could not find texture {} required by the imported model.", src.string());
      continue;
    }

    std::filesystem::path dst = IMPL.ctx.outDir / texPath;

    if (std::filesystem::exists(dst))
    {
      VRM_LOG_WARN("Texture {} already exists and will not be overriden.", dst.string());
      continue;
    }

    std::filesystem::path dstDir = dst; dstDir.remove_filename();

    try
    {
      if (!std::filesystem::exists(dstDir))
        std::filesystem::create_directories(dstDir);
      std::filesystem::copy_file(src, dst);
    }
    catch(const std::exception& e)
    {
      VRM_LOG_WARN("Could not copy {} to {}. Error log: {}", src.string(), dst.string(), e.what());
      continue;
    }
    
  }

  // Creating materials

  for (const auto& [matName, j] : IMPL.ctx.outMaterials)
  {
    std::filesystem::path matPath = IMPL.ctx.outDir / matName;
    std::ofstream ofs;
    ofs.open(matPath, std::fstream::trunc);

    if (!ofs.is_open())
    {
      VRM_LOG_WARN("Could not open/create material file {}", matPath.string());
      continue;
    }

    try
    {
      ofs << j;
    }
    catch (const std::exception& e)
    {
      VRM_LOG_WARN("Could not create material file {}. Error log: {}", matPath.string(), e.what());
    }
  }

  // Importing model object

  std::filesystem::copy(IMPL.ctx.inFile, IMPL.ctx.outDir, std::filesystem::copy_options::overwrite_existing);
  MetaFile metaData;
  metaData.Type = MetaFile::EType::eMesh;
  AssetUtils::CreateMetaFile(metaData, IMPL.ctx.outDir / meshFileName);
}

void ModelImporter::_processMaterial(aiMaterial* material)
{
  std::string matName = material->GetName().C_Str();

  if (IMPL.ctx.outMaterials.contains(matName))
    return;

  MaterialData data;
  data.setShadingModel(MaterialData::EShadingModel::ePhong);
  

  std::string diffuseMap = _getTexture(material, aiTextureType_DIFFUSE);
  std::string specularMap = _getTexture(material, aiTextureType_SPECULAR);
  std::string shininessMap = _getTexture(material, aiTextureType_SHININESS);
  
  MaterialData::Parameter p;

  if (diffuseMap.size() > 0)
  {
    _registerTexture(diffuseMap);
    p.name = "Diffuse";
    p.setValue(diffuseMap);
    data.addParameter(p);
  }
  
  if (specularMap.size() > 0)
  {
    _registerTexture(specularMap);
    p.name = "Specular";
    p.setValue(specularMap);
    data.addParameter(p);
  }
  
  if (shininessMap.size() > 0)
  {
    _registerTexture(shininessMap);
    p.name = "Specular";
    p.setValue(shininessMap);
    data.addParameter(p);
  }

  std::filesystem::path outPath = IMPL.ctx.outDir / matName;
  
  IMPL.ctx.outMaterials[matName] = data;
}

std::string ModelImporter::_getTexture(aiMaterial* material, aiTextureType type) const
{
  if (material->GetTextureCount(type) > 0)
  {
    aiString texName;
    material->GetTexture(type, 0, &texName);
    return texName.C_Str();
  }
  else
  {
    return "";
  }
}

void ModelImporter::_registerTexture(const std::string& texName)
{
  std::filesystem::path inPath = IMPL.ctx.inFile / texName;
  IMPL.ctx.texturesToImport.emplace(texName);
}
