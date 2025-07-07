#pragma once

#include <memory>
#include <filesystem>
#include <assimp/material.h>

struct aiNode;
struct aiMaterial;
struct aiTexture;
struct aiScene;

namespace vrm
{

  class ModelImporter
  {
  public:

    ModelImporter();
    ~ModelImporter();

    bool import(const std::filesystem::path& inPath, const std::filesystem::path& outPath);

  private:

    void _createFiles();

    void _processMaterialPhong(aiMaterial* material, unsigned int id);
    void _processMaterialPBR(aiMaterial* material, unsigned int id);

    std::string _getTexture(aiMaterial* material, aiTextureType type) const;
    void _registerTexture(const std::string& texName);

  private:
    
    struct Impl;
    std::unique_ptr<Impl> m_impl;
  
  };

} // namespace vrm
