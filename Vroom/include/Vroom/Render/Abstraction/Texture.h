#pragma once

#include <array>
#include <cmath>

#include "Vroom/Asset/AssetData/TextureData.h"
#include "Vroom/Core/Assert.h"
#include "Vroom/Render/Abstraction/GLCore.h"


namespace vrm::gl
{

class Texture
{
public:

  struct Desc
  {
    GLuint    dimension = 0;
    GLsizei   width = 0, height = 0, depth = 0;
    GLint     mipmapCount          = 1;
    GLint     internalFormat       = 0;
    GLenum    format               = 0;
    GLuint    sampleCount          = 1;
    GLboolean fixedSampleLocations = GL_TRUE;
    bool      layered              = false;
    bool      cubemap              = false;
  };

public:

  Texture();
  ~Texture();

  Texture& operator=(const Texture&) = delete;
  Texture(const Texture&)            = delete;

  Texture& operator=(Texture&&) = delete;
  Texture(Texture&&)            = delete;

  inline static constexpr GLenum ToGlFormat(TextureFormat format);
  inline static constexpr GLint  ToGlInternalFormat(TextureFormat format);
  inline static constexpr GLenum GetBindingTarget(GLuint layers,
                                                  GLuint samples);

  inline static GLint  GetMaxMipMapCount(GLsizei width, GLsizei height);
  inline static GLint  GetBasicColorInternalFormat(GLuint channelCount,
                                                   GLuint bitsPerChannel);
  inline static GLenum GetBasicColorFormat(GLuint channelCount);

  inline static constexpr GLuint GetChannelCountFromFormat(GLenum format);

  void create(const Desc& desc);
  void release();
  void bind(GLenum target) const;
  void bind() const;

  inline bool        isCreated() const { return m_renderId != 0; }
  inline GLuint      getRenderId() const { return m_renderId; }
  inline const Desc& getDescription() const { return m_desc; }
  inline GLenum      getDefaultTarget() const { return m_defaultTarget; }

private:

  GLuint m_renderId      = 0;
  GLenum m_defaultTarget = 0;
  Desc   m_desc          = {};
};

inline constexpr GLenum Texture::ToGlFormat(TextureFormat format)
{
  switch (format)
  {
  case TextureFormat::Grayscale:
    return GL_RED;
  case TextureFormat::RG:
    return GL_RG;
  case TextureFormat::RGB:
    return GL_RGB;
  case TextureFormat::RGBA:
    return GL_RGBA;
  default:
    VRM_ASSERT_MSG(false, "Unsupported format.");
    return 0;
  }
}

inline constexpr GLint Texture::ToGlInternalFormat(TextureFormat format)
{
  switch (format)
  {
  case TextureFormat::Grayscale:
    return GL_R8;
  case TextureFormat::RG:
    return GL_RG8;
  case TextureFormat::RGB:
    return GL_RGB8;
  case TextureFormat::RGBA:
    return GL_RGBA8;
  default:
    VRM_ASSERT_MSG(false, "Unsupported format.");
    return 0;
  }
}

inline constexpr GLenum Texture::GetBindingTarget(GLuint layers, GLuint samples)
{
  if (layers > 1)
  {
    return samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE_ARRAY : GL_TEXTURE_2D_ARRAY;
  }
  else { return samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D; }
}

inline GLint Texture::GetMaxMipMapCount(GLsizei width, GLsizei height)
{
  return 1 + static_cast<GLint>(std::floor(std::log2(std::max(width, height))));
}

inline GLint Texture::GetBasicColorInternalFormat(GLuint channelCount,
                                                  GLuint bitsPerChannel)
{
  static constexpr std::array<std::array<GLenum, 3>, 4> s_assoc = {
    { // 1 channel (RED)
      { GL_R8, GL_R16, GL_R32F },
     // 2 channels (RG)
      { GL_RG8, GL_RG16, GL_RG32F },
     // 3 channels (RGB)
      { GL_RGB8, GL_RGB16, GL_RGB32F },
     // 4 channels (RGBA)
      { GL_RGBA8, GL_RGBA16, GL_RGBA32F } }
  };

  size_t bitsIndex = 0;
  switch (bitsPerChannel)
  {
  case 8:
    bitsIndex = 0;
    break;
  case 16:
    bitsIndex = 1;
    break;
  case 32:
    bitsIndex = 2;
    break;
  default:
    throw std::out_of_range("bitsPerChannel must be 8, 16 or 32");
  }

  if (channelCount < 1 || channelCount > 4)
    throw std::out_of_range("channelCount must be in [1, 4]");

  return s_assoc[channelCount - 1][bitsIndex];
}

inline GLenum Texture::GetBasicColorFormat(GLuint channelCount)
{
  static std::array<GLenum, 4> s_assoc = { GL_RED, GL_RG, GL_RGB, GL_RGBA };

  if (channelCount < 1 || channelCount > 4)
    throw std::out_of_range("channelCount must be in [1, 4]");

  return s_assoc[channelCount - 1];
}

inline constexpr GLuint Texture::GetChannelCountFromFormat(GLenum format)
{
  switch (format)
  {
  case GL_RED:
    return 1;
  case GL_RG:
    return 2;
  case GL_RGB:
    return 3;
  case GL_RGBA:
    return 4;
  default:
    return -1;
  }
}

} // namespace vrm::gl
