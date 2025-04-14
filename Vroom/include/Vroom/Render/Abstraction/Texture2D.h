#pragma once

#include <vector>
#include <string>

#include "Vroom/Asset/AssetData/TextureData.h"

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

    void bind() const;
    void bind(unsigned int slot) const;
    void unbind() const;

    void createColors(int width, int height, int channels, const void *data = nullptr);

    void createColorsMultisample(int width, int height, int channels, int samples);

    void createFloats(int width, int height, int channels, const void *data = nullptr);

    void createDepth(int width, int height);

    void release();

    void loadFromTextureData(const ByteTextureData &textureData);

    void loadFromTextureData(const FloatTextureData &textureData);

    inline constexpr bool isCreated() const { return m_RendererID != 0; }

    inline constexpr unsigned int getWidth() const { return m_width; }

    inline constexpr unsigned int getHeight() const { return m_height; }

    inline constexpr unsigned int getChannels() const { return m_channels; }

    inline constexpr unsigned int getRendererID() const { return m_RendererID; }

  private:
    unsigned int m_RendererID = 0;
    unsigned int m_width = 0, m_height = 0;
    unsigned int m_channels = 0;
    unsigned int m_samples = 0;
  };

} // namespace vrm::gl