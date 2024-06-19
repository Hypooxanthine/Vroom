#pragma once

#include "Vroom/Asset/AssetInstance/AssetInstance.h"

namespace vrm
{

class MeshAsset;

class MeshInstance : public AssetInstance
{
public:
    MeshInstance(MeshAsset* meshAsset);
    ~MeshInstance() = default;

    MeshAsset* getStaticAsset();
};

} // namespace vrm

