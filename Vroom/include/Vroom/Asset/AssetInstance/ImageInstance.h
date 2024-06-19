#pragma once

#include "Vroom/Asset/AssetInstance/AssetInstance.h"

namespace vrm
{

class ImageAsset;

class ImageInstance : public AssetInstance
{
public:

    /**
     * @brief Constructor.
     * 
     * @param imageAsset The image asset that this instance is based on.
     */
    ImageInstance(ImageAsset* imageAsset);

    /**
     * @brief Destructor.
     * 
     */
    ~ImageInstance() = default;

    /**
     * @brief Get the static asset.
     * 
     * @return ImageAsset* The image asset.
     */
    ImageAsset* getStaticAsset();
};

} // namespace vrm