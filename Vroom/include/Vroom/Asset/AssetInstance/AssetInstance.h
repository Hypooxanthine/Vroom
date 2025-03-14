#pragma once

#include "Vroom/Asset/Macros.h"

namespace vrm
{

class StaticAsset;

/**
 * @brief  Base class for all asset instances.
 * 
 */
class AssetInstance
{
public:

    /**
     * @brief Default constructor. No static asset is set.
     * 
     */
    AssetInstance();

    /**
     * @brief Constructor.
     * 
     * @param staticAsset The static asset that this instance is based on.
     */
    AssetInstance(StaticAsset* staticAsset);

    /**
     * @brief Copy constructor.
     * 
     * @param other The other instance to copy.
     */
    AssetInstance(const AssetInstance& other);

    /**
     * @brief Copy assignment operator.
     * 
     * @param other The other instance to copy.
     * @return AssetInstance& The reference to this instance.
     */
    AssetInstance& operator=(const AssetInstance& other);

    /**
     * @brief  Move constructor.
     * 
     * @param other  The other instance to move.
     */
    AssetInstance(AssetInstance&& other);

    /**
     * @brief Move assignment operator.
     * 
     * @param other The other instance to move.
     * @return AssetInstance& The reference to this instance.
     */
    AssetInstance& operator=(AssetInstance&& other);

    /**
     * @brief Destructor.
     * 
     */
    virtual ~AssetInstance();

protected:
    StaticAsset* m_StaticAsset;
};

} // namespace vrm