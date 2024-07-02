#include "Vroom/Asset/StaticAsset/TextureAsset.h"

#include <stb_image/stb_image.h>

#include "Vroom/Core/Log.h"
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
    LOG_INFO("Loading texture: {}", filePath);
    
    if (!m_GPUTexture.loadFromFile(filePath))
    {
        LOG_ERROR("Failed to load texture: {}", filePath);
        return false;
    }

    LOG_INFO("Texture loaded.");

    return true;
}

}