#pragma once

#include <vector>
#include <string>

#include "Vroom/Asset/AssetData/TextureData.h"

namespace vrm
{

class Texture2D
{
public:
    Texture2D();
    
    virtual ~Texture2D();

    Texture2D& operator=(const Texture2D& other) = delete;
    Texture2D(const Texture2D& other) = delete;

    Texture2D& operator=(Texture2D&& other) noexcept;
    Texture2D(Texture2D&& other) noexcept;

    void bind(unsigned int slot = 0) const;
    void unbind() const;

    void createColors(int width, int height, int channels, const void* data = nullptr);

    void createFloats(int width, int height, int channels, const void* data = nullptr);

    void createDepth(int width, int height);

    void release();

    void loadFromTextureData(const ByteTextureData& textureData);

    void loadFromTextureData(const FloatTextureData& textureData);

    inline bool isCreated() const { return m_RendererID != 0; }

    inline unsigned int getRendererID() const { return m_RendererID; }

private:
    unsigned int m_RendererID = 0;
};

} // namespace vrm