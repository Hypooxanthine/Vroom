#pragma once

#include "Vroom/Api.h"
#include "Vroom/Asset/StaticAsset/StaticAsset.h"
#include "Vroom/Asset/StaticAsset/TextureAsset.h"
#include "Vroom/Asset/AssetData/MaterialData.h"
#include "Vroom/Asset/AssetData/ShaderData.h"
#include "Vroom/Render/Abstraction/Shader.h"

namespace vrm
{

  class VRM_API MaterialAsset : public StaticAsset
  {
  public:
    using Handle = AssetHandle<MaterialAsset>;

  public:
    MaterialAsset();
    ~MaterialAsset();

    [[nodiscard]] inline const MaterialData &getData() const { return m_data; }

    [[nodiscard]] inline const ShaderData& getShaderData() const { return m_materialShaderData; }

    [[nodiscard]] inline const std::vector<TextureAsset::Handle>& getTextures() const { return m_textures; }

    void applyUniforms(const gl::Shader& shader) const;

  protected:
    bool loadImpl(const std::string &filePath) override;

    bool buildShaderData();

  private:
    MaterialData m_data;
    std::vector<TextureAsset::Handle> m_textures;

    ShaderData m_materialShaderData;
  };

} // namespace vrm
