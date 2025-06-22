#include "Vroom/Render/Abstraction/Texture2D.h"

#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h"

#include "Vroom/Render/Abstraction/Texture.h"
#include "Vroom/Core/Assert.h"

using namespace vrm;
using namespace vrm::gl;

Texture2D::~Texture2D()
{
  release();
}

Texture2D &Texture2D::operator=(Texture2D &&other) noexcept
{
  if (this != &other)
  {
    m_RendererID = other.m_RendererID;
    m_width = other.m_width;
    m_height = other.m_height;
    m_channels = other.m_channels;
    m_samples = other.m_samples;
    m_textureFormat = other.m_textureFormat;

    other.m_RendererID = 0;
    other.m_width = 0;
    other.m_height = 0;
    other.m_channels = 0;
    other.m_samples = 0;
    other.m_textureFormat = TextureFormat::Unsupported;
  }

  return *this;
}

Texture2D::Texture2D(Texture2D &&other) noexcept
{
  *this = std::move(other);
}

void Texture2D::bind() const
{
  VRM_DEBUG_ASSERT_MSG(isCreated(), "Texture not created.");

  glBindTexture(getBindingTarget(), m_RendererID);
}

void Texture2D::bind(unsigned int slot) const
{
  glActiveTexture(GL_TEXTURE0 + slot);
  bind();
}

void Texture2D::unbind() const
{
  GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture2D::createColors(int width, int height, int channels, int samples, const void *data, bool genMipMaps)
{
  VRM_ASSERT_MSG(!(genMipMaps && samples > 1), "Mipmapping for multisampled textures is unsupported");

  if (!isCreated())
    glGenTextures(1, &m_RendererID);

  m_width = width;
  m_height = height;
  m_channels = channels;
  m_samples = samples;
  m_textureFormat = ChannelsToTextureFormat(channels);

  bind();

  GLenum target = getBindingTarget();
  
  if (samples > 1)
  {
     glTexImage2DMultisample(
      target,
      samples,
      Texture::ToGlInternalFormat(m_textureFormat),
      width,
      height,
      GL_TRUE
    );
   }
  else
  {
    glTexImage2D(
      target,
      0,
      Texture::ToGlInternalFormat(m_textureFormat),
      width, height,
      0,
      Texture::ToGlFormat(m_textureFormat),
      GL_UNSIGNED_BYTE,
      data
    );
  }

  if (genMipMaps)
  {
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  }
  else
  {
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  }

  glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Texture2D::createFloats(int width, int height, int channels, const void *data)
{
  if (!isCreated())
    glGenTextures(1, &m_RendererID);

  m_samples = 1;
  m_textureFormat = ChannelsToTextureFormat(channels);

  bind();
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
  GLCall(glTexImage2D(
      GL_TEXTURE_2D,
      0,
      Texture::ToGlInternalFormat(m_textureFormat),
      width,
      height,
      0,
      Texture::ToGlFormat(m_textureFormat),
      GL_FLOAT,
      data));

  m_width = width;
  m_height = height;
  m_channels = channels;
}

void Texture2D::createDepth(int width, int height)
{
  if (!isCreated())
    glGenTextures(1, &m_RendererID);

  m_samples = 1;
  m_textureFormat = TextureFormat::Depth;

  bind();
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
  GLCall(glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_DEPTH_COMPONENT,
      width,
      height,
      0,
      GL_DEPTH_COMPONENT,
      GL_FLOAT,
      nullptr));

  m_width = width;
  m_height = height;
  m_channels = 1;
}

void Texture2D::release()
{
  if (isCreated())
  {
    GLCall_nothrow(glDeleteTextures(1, &m_RendererID));
    m_RendererID = 0;
    m_width = 0;
    m_height = 0;
    m_channels = 0;
    m_samples = 0;
  }
}

void Texture2D::loadFromTextureData(const ByteTextureData &textureData)
{
  createColors(textureData.getWidth(), textureData.getHeight(), textureData.getChannels(), 1, textureData.getData(), true);
}

void Texture2D::loadFromTextureData(const FloatTextureData &textureData)
{
  createFloats(textureData.getWidth(), textureData.getHeight(), textureData.getChannels(), textureData.getData());
}
