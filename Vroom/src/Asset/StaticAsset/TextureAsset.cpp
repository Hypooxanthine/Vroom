#include "Vroom/Asset/StaticAsset/TextureAsset.h"

#include "Vroom/Asset/AssetInstance/TextureInstance.h"

#include "Vroom/Asset/AssetManager.h"

namespace vrm
{

TextureAsset::TextureAsset()
{
}

TextureAsset::~TextureAsset()
{
}

TextureInstance TextureAsset::createInstance()
{
    return TextureInstance(this);
}

bool TextureAsset::loadImpl(const std::string& filePath, AssetManager& manager)
{
    return true;
}

}