#include "Vroom/Asset/StaticAsset/MaterialAsset.h"

#include <glm/glm.hpp>
#include <unordered_set>

#include "Vroom/Core/Log.h"

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/Parsing/MaterialParsing.h"

namespace vrm
{

MaterialAsset::MaterialAsset()
    : StaticAsset()
{
}

MaterialAsset::~MaterialAsset()
{
}

MaterialInstance MaterialAsset::createInstance()
{
    return MaterialInstance(this);
}

bool MaterialAsset::loadImpl(const std::string& filePath, AssetManager& manager)
{
    VRM_LOG_INFO("Loading material: {}", filePath);

    auto shadersData = MaterialParsing::Parse(filePath);

    if (!m_Shader.loadFromSource(shadersData.vertex, shadersData.fragment))
    {
        VRM_LOG_ERROR("Failed to load material: {}", filePath);
        return false;
    }

    // Loading textures
    for (const std::string& texturePath : shadersData.texturePaths)
    {
        m_Textures.emplace_back(manager.getAsset<TextureAsset>(texturePath));
    }

    return true;
}

} // namespace vrm