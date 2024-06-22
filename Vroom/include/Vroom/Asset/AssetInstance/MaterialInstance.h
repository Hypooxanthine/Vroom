#pragma once

#include "Vroom/Asset/AssetInstance/AssetInstance.h"

namespace vrm
{

class MaterialAsset;

class MaterialInstance : public AssetInstance
{
public:
    MaterialInstance();
    MaterialInstance(MaterialAsset* materialAsset);
    ~MaterialInstance();
    
    MaterialAsset* getStaticAsset();
    const MaterialAsset* getStaticAsset() const;
};

} // namespace vrm