#include "Vroom/Render/Abstraction/Texture2D.h"

#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h"

#include "Vroom/Render/Abstraction/GLCall.h"
#include "Vroom/Core/Assert.h"

using namespace vrm;
using namespace vrm::gl;

static constexpr GLenum ToGlFormat(TextureFormat format)
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

static constexpr GLint ToGlInternalFormat(TextureFormat format)
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

    other.m_RendererID = 0;
    other.m_width = 0;
    other.m_height = 0;
    other.m_channels = 0;
    other.m_samples = 0;
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

  if (m_samples > 1)
  {
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_RendererID);
  }
  else
  {
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
  }
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

void Texture2D::createColors(int width, int height, int channels, const void *data)
{
  if (!isCreated())
    glGenTextures(1, &m_RendererID);

  m_samples = 1;

  bind();
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
  GLCall(glTexImage2D(
      GL_TEXTURE_2D,
      0,
      ToGlInternalFormat(ChannelsToTextureFormat(channels)),
      width,
      height,
      0,
      ToGlFormat(ChannelsToTextureFormat(channels)),
      GL_UNSIGNED_BYTE,
      data));
  
  glGenerateMipmap(GL_TEXTURE_2D);

  m_width = width;
  m_height = height;
  m_channels = channels;
}

void Texture2D::createColorsMultisample(int width, int height, int channels, int samples)
{
  VRM_ASSERT_MSG(samples > 1, "Samples must be greater than 1");

  if (!isCreated())
    glGenTextures(1, &m_RendererID);

  m_samples = samples;
  
  bind();
  glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2DMultisample(
    GL_TEXTURE_2D_MULTISAMPLE,
    samples,
    ToGlInternalFormat(ChannelsToTextureFormat(channels)),
    width,
    height,
    GL_TRUE
  );

  m_width = width;
  m_height = height;
  m_channels = channels;
}

void Texture2D::createFloats(int width, int height, int channels, const void *data)
{
  if (!isCreated())
    glGenTextures(1, &m_RendererID);

  m_samples = 1;

  bind();
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
  GLCall(glTexImage2D(
      GL_TEXTURE_2D,
      0,
      ToGlInternalFormat(ChannelsToTextureFormat(channels)),
      width,
      height,
      0,
      ToGlFormat(ChannelsToTextureFormat(channels)),
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
  createColors(textureData.getWidth(), textureData.getHeight(), textureData.getChannels(), textureData.getData());
}

void Texture2D::loadFromTextureData(const FloatTextureData &textureData)
{
  createFloats(textureData.getWidth(), textureData.getHeight(), textureData.getChannels(), textureData.getData());
}
