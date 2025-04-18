#pragma once

#include "Vroom/Render/Abstraction/GLCall.h"
#include "Vroom/Render/Abstraction/Texture.h"

namespace vrm::gl
{

  class ArrayTexture2D
  {
  public:
    inline constexpr ArrayTexture2D()
    {
    }

    inline ~ArrayTexture2D() noexcept
    {
      release();
    }

    inline static constexpr GLenum GetBindingTarget(GLuint samples)
    {
      return samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE_ARRAY : GL_TEXTURE_2D_ARRAY;
    }

    inline GLenum getBindingTarget() const
    {
      return GetBindingTarget(m_sampleCount);
    }

    inline constexpr bool isCreated() const
    {
      return m_renderID != 0;
    }

    inline constexpr GLuint getRenderID() const
    {
      return m_renderID;
    }

    inline constexpr GLuint getWidth() const
    {
      return m_width;
    }

    inline constexpr GLuint getHeight() const
    {
      return m_height;
    }

    inline constexpr GLuint getLayerCount() const
    {
      return m_layerCount;
    }

    inline constexpr GLuint getChannels() const
    {
      return m_channels;
    }

    inline constexpr GLuint getSamples() const
    {
      return m_sampleCount;
    }

    inline void bind() const
    {
      glBindTexture(getBindingTarget(), m_renderID);
    }

    inline constexpr void release() noexcept
    {
      if (isCreated())
      {
        glDeleteTextures(1, &m_renderID);
        m_renderID = 0;
        m_width = 0;
        m_height = 0;
        m_channels = 0;
        m_layerCount = 0;
        m_sampleCount = 0;
      }
    }

    inline void createColor(GLsizei width, GLsizei height, GLuint channels, GLsizei layerCount, GLuint sampleCount = 1)
    {
      if (!isCreated())
      {
        glGenTextures(1, &m_renderID);
      }

      m_layerCount = layerCount;
      m_width = width;
      m_height = height;
      m_channels = channels;
      m_sampleCount = sampleCount;

      bind();

      GLenum target = getBindingTarget();
      
      if (sampleCount > 1)
      {
        glTexStorage3DMultisample(target, sampleCount, Texture::ToGlInternalFormat(ChannelsToTextureFormat(channels)), width, height, layerCount, GL_TRUE);
      }
      else
      {
        glTexStorage3D(target, 0, Texture::ToGlInternalFormat(ChannelsToTextureFormat(channels)), width, height, layerCount);
      }

      glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    inline void createDepth(GLsizei width, GLsizei height, GLsizei layerCount)
    {
      if (!isCreated())
      {
        glGenTextures(1, &m_renderID);
      }

      m_layerCount = layerCount;
      m_width = width;
      m_height = height;
      m_channels = 1;
      m_sampleCount = 1;

      GLenum target = getBindingTarget();

      bind();

      glTexStorage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT, width, height, layerCount);

      glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

  protected:

  private:
    GLuint m_renderID = 0;
    GLuint m_width = 0;
    GLuint m_height = 0;
    GLuint m_channels = 0;
    GLuint m_layerCount = 0;
    GLuint m_sampleCount = 0;
  };

} // namespace vrm::gl
