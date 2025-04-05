#pragma once

#include "Vroom/Asset/StaticAsset/StaticAsset.h"
#include "Vroom/Asset/StaticAsset/TextureAsset.h"
#include "Vroom/Asset/AssetData/MaterialData.h"
#include "Vroom/Asset/AssetData/ShaderData.h"
#include "Vroom/Render/Abstraction/Shader.h"

namespace vrm
{

  class MaterialAsset : public StaticAsset
  {
  public:
    using Handle = AssetHandle<MaterialAsset>;

  public:
    MaterialAsset();
    ~MaterialAsset();

    [[nodiscard]] inline const MaterialData &getData() const { return m_data; }

    [[nodiscard]] inline const gl::Shader &getShader() const { return m_gpuShader; }

    [[nodiscard]] inline const std::vector<TextureAsset::Handle>& getTextures() const { return m_textures; }

    void applyUniforms() const;

  protected:
    bool loadImpl(const std::string &filePath) override;

    bool buildShader();

  private:
    MaterialData m_data;
    ShaderData m_shaderData;
    std::vector<TextureAsset::Handle> m_textures;

    gl::Shader m_gpuShader;
  };

} // namespace vrm
