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

  /**
   * @brief (Re)builds this material from in-memory data instead of a file.
   *
   * Useful for previewing unsaved edits without touching the asset on disk.
   * Bumps the generation on success so consumers caching by generation refresh.
   *
   * @param data        The material data to build from.
   * @param sourcePath  Path the data originates from, used to resolve relative
   *                    resources (e.g. textures) the same way a file load would.
   *                    Optional; leave empty to resolve relative to the working
   *                    directory.
   * @return true on success.
   */
  bool loadFromData(const MaterialData& data, const std::filesystem::path& sourcePath = {});

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

  // Builds all derived state (shaders, textures, ...) from m_data. Shared by
  // the file-based loadImpl and the in-memory loadFromData.
  bool buildFromData(const MaterialData& data);

  bool buildShaderData();

private:

  MaterialData                      m_data;
  std::vector<TextureAsset::Handle> m_textures;

  ShaderData                m_materialShaderData;
  ShadingModelAsset::Handle m_shadingModel;
};

} // namespace vrm
