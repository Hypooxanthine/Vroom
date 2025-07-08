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
    std::filesystem::path inFile, inDir, outDir;
    const aiScene* scene = nullptr;
    std::unordered_map<std::string, json> outMaterials;
    std::unordered_set<std::filesystem::path> filesToCopy;
    size_t genMatId = 0;
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
  IMPL.ctx.inDir = inPath; IMPL.ctx.inDir.remove_filename();
  IMPL.ctx.outDir = outPath;
  IMPL.ctx.scene = scene;

  // Check if the imported file is a GLTF file and copy .bin file if it exists
  std::filesystem::path binFile = IMPL.ctx.inFile;
  binFile.replace_extension(".bin");
  if (binFile.extension() == ".bin" && std::filesystem::exists(binFile))
  {
    binFile = binFile.lexically_relative(IMPL.ctx.inDir);
    IMPL.ctx.filesToCopy.emplace(binFile);
  }

  for (unsigned int i = 0; i < IMPL.ctx.scene->mNumMaterials; ++i)
  {
    aiMaterial* mat = IMPL.ctx.scene->mMaterials[i];
    aiShadingMode shadingModel;
    mat->Get(AI_MATKEY_SHADING_MODEL, shadingModel);

    if (shadingModel == aiShadingMode_PBR_BRDF)
      _processMaterialPBR(mat, i);
    else
      _processMaterialPhong(mat, i);
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

  for (const std::filesystem::path& filePath : IMPL.ctx.filesToCopy)
  {
    std::filesystem::path src = std::filesystem::canonical(IMPL.ctx.inDir / filePath);

    if (!std::filesystem::exists(src))
    {
      VRM_LOG_WARN("Could not find texture {} required by the imported model.", src.string());
      continue;
    }

    std::filesystem::path dst = IMPL.ctx.outDir / filePath;

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

      // VRM_LOG_INFO("Copying {} to {}", src.string(), dst.string());
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
      ofs.close();
      MetaFile metaData;
      metaData.Type = MetaFile::EType::eMaterial;
      AssetUtils::CreateMetaFile(metaData, matPath);
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

void ModelImporter::_processMaterialPhong(aiMaterial* material, unsigned int id)
{
  std::string matName = material->GetName().C_Str();
    
  if (matName.empty())
  {
    matName = "GenMat_" + std::to_string(id);
  }

  MaterialData data;
  data.setShadingModel(MaterialData::EShadingModel::ePhong);
  

  std::string diffuseMap = _getTexture(material, aiTextureType_DIFFUSE);
  std::string specularMap = _getTexture(material, aiTextureType_SPECULAR);
  std::string shininessMap = _getTexture(material, aiTextureType_SHININESS);
  
  MaterialData::Parameter p;
  
  p.name = "Diffuse";
  if (diffuseMap.size() > 0)
  {
    _registerTexture(diffuseMap);
    p.setValue(diffuseMap);
    data.addParameter(p);
  }
  else
  {
    aiColor4D col;
    material->Get(AI_MATKEY_COLOR_DIFFUSE, col);
    p.setValue(glm::vec3(col.r, col.g, col.b));
    data.addParameter(p);
  }
  
  p.name = "Specular";
  if (specularMap.size() > 0)
  {
    _registerTexture(specularMap);
    p.setValue(specularMap);
    data.addParameter(p);
  }
  else
  {
    aiColor4D col;
    material->Get(AI_MATKEY_COLOR_SPECULAR, col);
    p.setValue(glm::vec3(col.r, col.g, col.b));
    data.addParameter(p);
  }
   
  p.name = "Shininess";
  if (shininessMap.size() > 0)
  {
    _registerTexture(shininessMap);
    p.setValue(shininessMap);
    data.addParameter(p);
  }
  else
  {
    float col;
    material->Get(AI_MATKEY_SHININESS, col);
    p.setValue(col);
    data.addParameter(p);
  }

  std::filesystem::path outPath = IMPL.ctx.outDir / matName;
  
  IMPL.ctx.outMaterials[matName] = data;
}

void ModelImporter::_processMaterialPBR(aiMaterial* material, unsigned int id)
{
  std::string matName = material->GetName().C_Str();
    
  if (matName.empty())
  {
    matName = "GenMat_" + std::to_string(id);
  }

  MaterialData data;
  data.setShadingModel(MaterialData::EShadingModel::ePBR);
  

  std::string albedoMap = _getTexture(material, aiTextureType_BASE_COLOR);
  std::string metalnessMap = _getTexture(material, aiTextureType_METALNESS);
  std::string roughnessMap = _getTexture(material, aiTextureType_DIFFUSE_ROUGHNESS);
  
  MaterialData::Parameter p;
  
  p.name = "Albedo";
  if (albedoMap.size() > 0)
  {
    _registerTexture(albedoMap);
    p.setValue(albedoMap);
    data.addParameter(p);
  }
  else
  {
    aiColor4D col;
    material->Get(AI_MATKEY_COLOR_DIFFUSE, col);
    p.setValue(glm::vec3(col.r, col.g, col.b));
    data.addParameter(p);
  }
   
  p.name = "Metalness";
  if (metalnessMap.size() > 0)
  {
    _registerTexture(metalnessMap);
    p.setValue(metalnessMap);
    data.addParameter(p);
  }
  else
  {
    float col;
    material->Get(AI_MATKEY_METALLIC_FACTOR, col);
    p.setValue(col);
    data.addParameter(p);
  }
   
  p.name = "Roughness";
  if (roughnessMap.size() > 0)
  {
    _registerTexture(roughnessMap);
    p.setValue(roughnessMap);
    data.addParameter(p);
  }
  else
  {
    float col;
    material->Get(AI_MATKEY_ROUGHNESS_FACTOR, col);
    p.setValue(col);
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
  IMPL.ctx.filesToCopy.emplace(texName);
}
