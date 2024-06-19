#include "Vroom/Asset/AssetInstance/ImageInstance.h"

#include "Vroom/Asset/StaticAsset/ImageAsset.h"

namespace vrm
{

ImageInstance::ImageInstance(ImageAsset* imageAsset)
    : AssetInstance(imageAsset)
{
}

ImageAsset* ImageInstance::getStaticAsset()
{
    return static_cast<ImageAsset*>(m_StaticAsset);
}

} // namespace vrm