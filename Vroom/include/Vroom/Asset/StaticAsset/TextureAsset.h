#pragma once

#include "Vroom/Asset/StaticAsset/StaticAsset.h"
#include "Vroom/Asset/AssetInstance/TextureInstance.h"
#include "Vroom/Render/Abstraction/ImageTexture.h"

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

    [[nodiscard]] inline const ImageTexture& getGPUTexture() const { return m_GPUTexture; }

protected:
    bool loadImpl(const std::string& filePath) override;

private:
    ImageTexture m_GPUTexture;
};

} // namespace vrm