#pragma once

#include "Vroom/Api.h"
#include "Vroom/Asset/StaticAsset/StaticAsset.h"
#include "Vroom/Render/Abstraction/Texture.h"

namespace vrm
{

  class VRM_API TextureAsset : public StaticAsset
  {
    public:
      using Handle = AssetHandle<TextureAsset>;
  public:
    TextureAsset();
    ~TextureAsset();

    [[nodiscard]] inline const gl::Texture &getGPUTexture() const { return m_GPUTexture; }

    [[nodiscard]] inline const ByteTextureData &getTextureData() const { return m_TextureData; }

  protected:
    bool loadImpl(const std::string &filePath) override;

  private:
    ByteTextureData m_TextureData;
    gl::Texture m_GPUTexture;
  };

} // namespace vrm