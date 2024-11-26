#include "Vroom/Asset/StaticAsset/TextureAsset.h"

#include <stb_image/stb_image.h>

#include "Vroom/Core/Log.h"
#include "Vroom/Asset/AssetInstance/TextureInstance.h"

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

bool TextureAsset::loadImpl(const std::string& filePath)
{
    VRM_LOG_INFO("Loading texture: {}", filePath);
    
    if (!m_TextureData.loadFromFile(filePath))
    {
        VRM_LOG_ERROR("Failed to load texture: {}", filePath);
        return false;
    }

    m_GPUTexture.loadFromTextureData(m_TextureData);

    VRM_LOG_INFO("Texture loaded.");

    return true;
}

}