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
    auto shadersData = MaterialParsing::Parse(filePath);

    return m_Shader.loadFromSource(shadersData.vertex, shadersData.fragment);
}

} // namespace vrm