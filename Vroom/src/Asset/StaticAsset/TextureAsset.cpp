#include "Vroom/Asset/StaticAsset/TextureAsset.h"

#include "Vroom/Core/Log.h"

using namespace vrm;

TextureAsset::TextureAsset()
{
}

TextureAsset::~TextureAsset()
{
}

bool TextureAsset::loadImpl(const std::string& filePath)
{
  VRM_LOG_TRACE("Loading texture: {}", filePath);

  if (!m_TextureData.loadFromFile(filePath))
  {
    VRM_LOG_ERROR("Failed to load texture: {}", filePath);
    return false;
  }
  
  gl::Texture::Desc desc;
  {
    desc.dimension = 2;
    desc.width = m_TextureData.getWidth();
    desc.height = m_TextureData.getHeight();
    desc.mipmapCount = gl::Texture::GetMaxMipMapCount(desc.width, desc.height);

    switch (m_TextureData.getChannels())
    {
    case 1:
      desc.internalFormat = GL_R8;
      desc.format = GL_R;
      break;
    case 2:
      desc.internalFormat = GL_RG8;
      desc.format = GL_RG;
      break;
    case 3:
      desc.internalFormat = GL_RGB8;
      desc.format = GL_RGB;
      break;
    case 4:
      desc.internalFormat = GL_RGBA8;
      desc.format = GL_RGBA;
      break;
    default:
      VRM_ASSERT_MSG(false, "Unexpected texture channel count");
      break;
    }
  }

  m_GPUTexture.create(desc);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, desc.width, desc.height, desc.format, GL_UNSIGNED_BYTE, m_TextureData.getData());
  glGenerateMipmap(GL_TEXTURE_2D);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  return true;
}
