#include "AssetManager/TextureAsset.h"

#include "Core/Log.h"

using namespace vrm;

TextureAsset::TextureAsset()
{}

TextureAsset::~TextureAsset()
{}

bool TextureAsset::loadImpl(const std::string& filePath)
{
  VRM_LOG_TRACE("Loading texture: {}", filePath);

  // Load CPU texture data from file
  if (!m_TextureData.loadFromFile(filePath))
  {
    VRM_LOG_ERROR("Failed to load texture: {}", filePath);
    return false;
  }

  // Create GPU texture descriptor based on texture data and load descriptor
  gl::Texture::Desc gpuDesc;
  {
    gpuDesc.dimension   = 2;
    gpuDesc.width       = m_TextureData.getWidth();
    gpuDesc.height      = m_TextureData.getHeight();
    gpuDesc.mipmapCount = gl::Texture::GetMaxMipMapCount(gpuDesc.width, gpuDesc.height);

    // Use descriptor's bit depth, or default to 8
    GLuint bitsPerChannel = m_LoadDescriptor.bitsPerChannel;
    GLuint channelCount   = m_TextureData.getChannels();

    gpuDesc.internalFormat = gl::Texture::GetBasicColorInternalFormat(channelCount, bitsPerChannel);
    gpuDesc.format         = gl::Texture::GetBasicColorFormat(channelCount);

    if (gpuDesc.internalFormat < 0 || gpuDesc.format < 0)
    {
      VRM_LOG_ERROR("Invalid texture format configuration for channels={}, bits={}", channelCount, bitsPerChannel);
      return false;
    }
  }

  // Create GPU texture storage
  m_GPUTexture.create(gpuDesc);
  m_GPUTexture.uploadData(m_TextureData.getData(), m_LoadDescriptor.samplingDesc);

  // Optionally release CPU data if not needed
  if (!m_LoadDescriptor.keepCPUData)
  {
    m_TextureData.release();
  }

  VRM_LOG_TRACE("Texture loaded successfully: {} ({}x{}, channels={})", filePath, gpuDesc.width, gpuDesc.height,
                m_TextureData.getChannels());

  return true;
}
