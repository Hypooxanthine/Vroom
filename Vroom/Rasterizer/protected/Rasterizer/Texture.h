#pragma once

#include "Rasterizer/Api.h"
#include "Rasterizer/GLCore.h"

namespace vrm::gl
{

class VRM_RASTERIZER_API Texture
{
public:

  // Texture sampling and wrapping configuration
  struct SamplingDesc
  {
    GLenum minFilter = GL_LINEAR_MIPMAP_LINEAR;
    GLenum magFilter = GL_LINEAR;
    GLenum wrapS     = GL_MIRRORED_REPEAT;
    GLenum wrapT     = GL_MIRRORED_REPEAT;
    GLenum wrapR     = GL_MIRRORED_REPEAT;
  };

  // GPU memory layout descriptor
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

  Texture& operator=(Texture&& other);
  Texture(Texture&& other);

  inline static constexpr GLenum GetBindingTarget(GLuint layers, GLuint samples);

  static GLint  GetMaxMipMapCount(GLsizei width, GLsizei height);
  static GLint  GetBasicColorInternalFormat(GLuint channelCount, GLuint bitsPerChannel);
  static GLenum GetBasicColorFormat(GLuint channelCount);

  inline static constexpr GLuint GetChannelCountFromFormat(GLenum format);

  void create(const Desc& desc);
  void release();

  // Upload texture data from CPU to GPU and apply sampling parameters
  void uploadData(const void* data);
  void uploadData(const void* data, const SamplingDesc& sampling);

  void bind(GLenum target) const;
  void bind() const;

  bool        isCreated() const;
  GLuint      getRenderId() const;
  const Desc& getDescription() const;
  GLenum      getDefaultTarget() const;

private:

  GLuint m_renderId      = 0;
  GLenum m_defaultTarget = 0;
  Desc   m_desc          = {};
};

inline constexpr GLenum Texture::GetBindingTarget(GLuint layers, GLuint samples)
{
  if (layers > 1)
  {
    return samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE_ARRAY : GL_TEXTURE_2D_ARRAY;
  }
  else
  {
    return samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
  }
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
