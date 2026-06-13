#pragma once

#include "AssetManager/Api.h"
#include "AssetManager/MaterialData.h"
#include "AssetManager/ShaderData.h"
#include "AssetManager/ShadingModelAsset.h"
#include "AssetManager/StaticAsset.h"
#include "AssetManager/TextureAsset.h"
#include "Rasterizer/Shader.h"

namespace vrm
{

class VRM_ASSETMANAGER_API MaterialAsset : public StaticAsset
{
public:

  using Handle = AssetHandle<MaterialAsset>;

public:

  MaterialAsset();
  ~MaterialAsset();

  inline const MaterialData& getData() const
  {
    return m_data;
  }

  inline const ShaderData& getShaderData() const
  {
    return m_materialShaderData;
  }

  inline ShadingModelAsset::Handle getShadingModel() const
  {
    return m_shadingModel;
  }

  inline const std::vector<TextureAsset::Handle>& getTextures() const
  {
    return m_textures;
  }

  void applyUniforms(const gl::Shader& shader) const;

protected:

  bool loadImpl(const std::filesystem::path& filePath) override;

  bool buildShaderData();

private:

  MaterialData                      m_data;
  std::vector<TextureAsset::Handle> m_textures;

  ShaderData                m_materialShaderData;
  ShadingModelAsset::Handle m_shadingModel;
};

} // namespace vrm
