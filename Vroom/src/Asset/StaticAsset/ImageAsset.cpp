#include "Vroom/Asset/StaticAsset/ImageAsset.h"

#include "Vroom/Asset/AssetInstance/ImageInstance.h"

namespace vrm
{

ImageInstance ImageAsset::createInstance()
{
    return ImageInstance(this);
}

bool ImageAsset::loadImpl(const std::string& filePath)
{
    // Load image from file
    return true;
}

} // namespace vrm