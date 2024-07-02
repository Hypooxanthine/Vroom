#pragma once

#include "Vroom/Asset/StaticAsset/StaticAsset.h"
#include "Vroom/Asset/AssetInstance/TextureInstance.h"
#include "Vroom/Render/Abstraction/Texture.h"

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

    [[nodiscard]] inline const Texture& getGPUTexture() const { return m_GPUTexture; }

protected:
    bool loadImpl(const std::string& filePath, AssetManager& manager) override;

private:
    Texture m_GPUTexture;
};

} // namespace vrm