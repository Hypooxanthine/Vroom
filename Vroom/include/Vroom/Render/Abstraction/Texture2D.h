#pragma once

#include <vector>
#include <string>

#include "Vroom/Asset/AssetData/TextureData.h"
#include "Vroom/Render/Abstraction/GLCall.h"

namespace vrm::gl
{

  class Texture2D
  {
  public:
    constexpr Texture2D() {}

    virtual ~Texture2D();

    Texture2D &operator=(const Texture2D &other) = delete;
    Texture2D(const Texture2D &other) = delete;

    Texture2D &operator=(Texture2D &&other) noexcept;
    Texture2D(Texture2D &&other) noexcept;

    inline static constexpr GLenum GetBindingTarget(GLuint samples)
    {
      return samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    }

    inline GLenum getBindingTarget() const
    {
      return GetBindingTarget(m_samples);
    }

    void bind() const;
    void bind(unsigned int slot) const;
    void unbind() const;

    void createColors(int width, int height, int channels, int samples = 1, const void *data = nullptr, bool genMipMaps = false);

    void createFloats(int width, int height, int channels, const void *data = nullptr);

    void createDepth(int width, int height);

    void release();

    void loadFromTextureData(const ByteTextureData &textureData);

    void loadFromTextureData(const FloatTextureData &textureData);

    inline constexpr bool isCreated() const { return m_RendererID != 0; } 

    inline constexpr unsigned int getWidth() const { return m_width; }

    inline constexpr unsigned int getHeight() const { return m_height; }

    inline constexpr unsigned int getChannels() const { return m_channels; }

    inline constexpr unsigned int getSamples() const { return m_samples; }

    inline constexpr unsigned int getRendererID() const { return m_RendererID; }

  private:
    unsigned int m_RendererID = 0;
    unsigned int m_width = 0, m_height = 0;
    unsigned int m_channels = 0;
    unsigned int m_samples = 0;
  };

} // namespace vrm::gl