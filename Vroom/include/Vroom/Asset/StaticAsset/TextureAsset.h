#pragma once

#include "Vroom/Asset/StaticAsset/StaticAsset.h"
#include "Vroom/Render/Abstraction/Texture2D.h"

namespace vrm
{

  class TextureAsset : public StaticAsset
  {
    public:
      using Handle = AssetHandle<TextureAsset>;
  public:
    TextureAsset();
    ~TextureAsset();

    [[nodiscard]] inline const gl::Texture2D &getGPUTexture() const { return m_GPUTexture; }

    [[nodiscard]] inline const ByteTextureData &getTextureData() const { return m_TextureData; }

  protected:
    bool loadImpl(const std::string &filePath) override;

  private:
    ByteTextureData m_TextureData;
    gl::Texture2D m_GPUTexture;
  };

} // namespace vrm