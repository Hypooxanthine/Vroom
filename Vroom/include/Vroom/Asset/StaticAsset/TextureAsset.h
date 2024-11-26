#pragma once

#include "Vroom/Asset/StaticAsset/StaticAsset.h"
#include "Vroom/Asset/AssetInstance/TextureInstance.h"
#include "Vroom/Render/Abstraction/Texture2D.h"

namespace vrm
{

class TextureAsset : public StaticAsset
{
public:
    using InstanceType = TextureInstance;

public:
    TextureAsset();
    ~TextureAsset();

    [[nodiscard]] TextureInstance createInstance();

    [[nodiscard]] inline const Texture2D& getGPUTexture() const { return m_GPUTexture; }

    [[nodiscard]] inline const ByteTextureData& getTextureData() const { return m_TextureData; }

protected:
    bool loadImpl(const std::string& filePath) override;

private:
    ByteTextureData m_TextureData;
    Texture2D m_GPUTexture;
};

} // namespace vrm