#include "Vroom/Asset/AssetInstance/TextureInstance.h"

#include "Vroom/Asset/StaticAsset/TextureAsset.h"

namespace vrm
{

TextureInstance::TextureInstance()
    : AssetInstance()
{
}

TextureInstance::TextureInstance(TextureAsset* textureAsset)
    : AssetInstance(textureAsset)
{
}

TextureAsset* TextureInstance::getStaticAsset()
{
    return static_cast<TextureAsset*>(m_StaticAsset);
}

const TextureAsset* TextureInstance::getStaticAsset() const
{
    return static_cast<TextureAsset*>(m_StaticAsset);
}

} // namespace vrm