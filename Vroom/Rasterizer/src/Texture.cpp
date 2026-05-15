#include "Rasterizer/Texture.h"
#include <array>
#include <cmath>

using namespace vrm::gl;

Texture::Texture()
{}

Texture::~Texture()
{
  release();
}

Texture::Texture(Texture&& other)
{
  *this = std::move(other);
}

Texture& Texture::operator=(Texture&& other)
{
  if (this != &other)
  {
    m_renderId      = other.m_renderId;
    m_defaultTarget = other.m_defaultTarget;
    m_desc          = std::move(other.m_desc);

    other.m_renderId      = 0;
    other.m_defaultTarget = 0;
    other.m_desc          = {};
  }

  return *this;
}

GLint Texture::GetMaxMipMapCount(GLsizei width, GLsizei height)
{
  return 1 + static_cast<GLint>(std::floor(std::log2(std::max(width, height))));
}

GLint Texture::GetBasicColorInternalFormat(GLuint channelCount, GLuint bitsPerChannel)
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
    assert(false && "bitsPerChannel must be 8, 16 or 32");
    return -1;
  }

  if (channelCount < 1 || channelCount > 4)
  {
    assert(false && "channelCount must be in [1, 4]");
    return -1;
  }

  return s_assoc[channelCount - 1][bitsIndex];
}

GLenum Texture::GetBasicColorFormat(GLuint channelCount)
{
  static std::array<GLenum, 4> s_assoc = { GL_RED, GL_RG, GL_RGB, GL_RGBA };

  if (channelCount < 1 || channelCount > 4)
  {
    assert(false && "channelCount must be in [1, 4]");
    return -1;
  }

  return s_assoc[channelCount - 1];
}

bool Texture::isCreated() const
{
  return m_renderId != 0;
}

GLuint Texture::getRenderId() const
{
  return m_renderId;
}

const Texture::Desc& Texture::getDescription() const
{
  return m_desc;
}

GLenum Texture::getDefaultTarget() const
{
  return m_defaultTarget;
}

void Texture::release()
{
  if (m_renderId == 0)
    return;

  glDeleteTextures(1, &m_renderId);
  m_renderId      = 0;
  m_defaultTarget = 0;
  m_desc          = {};
}

void Texture::create(const Desc& desc)
{
  release();
  glGenTextures(1, &m_renderId);

  m_desc = desc;

  switch (desc.dimension)
  {
  case 1:
  {
    m_defaultTarget = desc.layered ? GL_TEXTURE_1D_ARRAY : GL_TEXTURE_1D;
    bind();
    glTexStorage1D(m_defaultTarget, desc.mipmapCount, desc.internalFormat, desc.width);
    break;
  }
  case 2:
  {
    if (desc.sampleCount == 1) // Not multisampled
    {
      if (desc.layered) // Array
      {
        if (desc.cubemap) // Cubemap array
        {
          m_defaultTarget = GL_TEXTURE_CUBE_MAP_ARRAY;
          bind();
          glTexStorage3D(m_defaultTarget, desc.mipmapCount, desc.internalFormat, desc.width, desc.height, desc.depth);
        }
        else // 2D texture array
        {
          m_defaultTarget = GL_TEXTURE_2D_ARRAY;
          bind();
          glTexStorage3D(m_defaultTarget, desc.mipmapCount, desc.internalFormat, desc.width, desc.height, desc.depth);
        }
      }
      else
      {
        if (desc.cubemap) // Cubemap
        {
          m_defaultTarget = GL_TEXTURE_CUBE_MAP;
          bind();
          glTexStorage2D(m_defaultTarget, desc.mipmapCount, desc.internalFormat, desc.width, desc.height);
        }
        else // 2D texture
        {
          m_defaultTarget = GL_TEXTURE_2D;
          bind();
          glTexStorage2D(m_defaultTarget, desc.mipmapCount, desc.internalFormat, desc.width, desc.height);
        }
      }
    }
    else // Multisampled
    {
      if (desc.layered) // Multisample 2D texture array
      {
        m_defaultTarget = GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
        bind();
        glTexStorage3DMultisample(m_defaultTarget, desc.sampleCount, desc.internalFormat, desc.width, desc.height,
                                  desc.depth, desc.fixedSampleLocations);
      }
      else // Multisample 2D texture
      {
        m_defaultTarget = GL_TEXTURE_2D_MULTISAMPLE;
        bind();
        glTexStorage2DMultisample(m_defaultTarget, desc.sampleCount, desc.internalFormat, desc.width, desc.height,
                                  desc.fixedSampleLocations);
      }
    }
    break;
  }
  case 3:
  {
    if (desc.sampleCount == 1)
    {
      m_defaultTarget = GL_TEXTURE_3D;
      bind();
      glTexStorage3D(m_defaultTarget, desc.mipmapCount, desc.internalFormat, desc.width, desc.height, desc.depth);
    }
    break;
  }
  }
}

void Texture::bind(GLenum target) const
{
  glBindTexture(target, m_renderId);
}

void Texture::bind() const
{
  bind(m_defaultTarget);
}

void Texture::uploadData(const void* data, const SamplingDesc& sampling)
{
  if (!isCreated() || m_desc.dimension != 2)
  {
    assert(false && "Texture must be created as 2D before uploading data");
    return;
  }

  bind();
  glTexSubImage2D(m_defaultTarget, 0, 0, 0, m_desc.width, m_desc.height, m_desc.format, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(m_defaultTarget);

  // Apply sampling parameters
  glTexParameteri(m_defaultTarget, GL_TEXTURE_MIN_FILTER, sampling.minFilter);
  glTexParameteri(m_defaultTarget, GL_TEXTURE_MAG_FILTER, sampling.magFilter);
  glTexParameteri(m_defaultTarget, GL_TEXTURE_WRAP_S, sampling.wrapS);
  glTexParameteri(m_defaultTarget, GL_TEXTURE_WRAP_T, sampling.wrapT);
  glTexParameteri(m_defaultTarget, GL_TEXTURE_WRAP_R, sampling.wrapR);
}
