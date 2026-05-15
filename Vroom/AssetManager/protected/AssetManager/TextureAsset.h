#pragma once

#include "AssetManager/Api.h"
#include "AssetManager/StaticAsset.h"
#include "AssetManager/TextureData.h"
#include "Rasterizer/Texture.h"

namespace vrm
{

// Descriptor for configuring texture loading and GPU parameters
struct TextureLoadDescriptor
{
  // GPU sampling configuration
  gl::Texture::SamplingDesc samplingDesc;

  // Whether to keep CPU data after GPU upload (default: true for post-processing, false to save memory)
  bool keepCPUData = true;

  // Bit depth per channel (8, 16, or 32)
  GLuint bitsPerChannel = 8;
};

class VRM_ASSETMANAGER_API TextureAsset : public StaticAsset
{
public:

  using Handle = AssetHandle<TextureAsset>;

public:

  TextureAsset();
  ~TextureAsset();

  // Configure how this texture should be loaded (call before loadImpl)
  inline void setLoadDescriptor(const TextureLoadDescriptor& desc)
  {
    m_LoadDescriptor = desc;
  }

  [[nodiscard]] inline const gl::Texture& getGPUTexture() const
  {
    return m_GPUTexture;
  }

  [[nodiscard]] inline const ByteTextureData& getTextureData() const
  {
    return m_TextureData;
  }

  // Release CPU texture data to free memory (only if keepCPUData was false)
  inline void releaseCPUData()
  {
    m_TextureData.release();
  }

protected:

  bool loadImpl(const std::string& filePath) override;

private:

  ByteTextureData       m_TextureData;
  gl::Texture           m_GPUTexture;
  TextureLoadDescriptor m_LoadDescriptor;
};

} // namespace vrm
