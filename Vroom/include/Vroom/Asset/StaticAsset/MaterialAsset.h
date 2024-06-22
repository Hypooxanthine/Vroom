#pragma once

#include "Vroom/Asset/StaticAsset/StaticAsset.h"
#include "Vroom/Asset/AssetInstance/MaterialInstance.h"
#include "Vroom/Asset/AssetInstance/ShaderInstance.h"

#include "Vroom/Asset/AssetData/MaterialData.h"

#include <fstream>

namespace vrm
{

class MaterialAsset : public StaticAsset
{
public:
    using InstanceType = MaterialInstance;
public:
    MaterialAsset();
    ~MaterialAsset();

    [[nodiscard]] MaterialInstance createInstance();

    [[nodiscard]] const MaterialData& getMaterialData() const { return m_MaterialData; }
    [[nodiscard]] const ShaderInstance& getShaderInstance() const { return m_ShaderInstance; }

protected:
    bool loadImpl(const std::string& filePath, AssetManager& manager) override;

private:
    bool loadPhong(std::ifstream& materialFile, AssetManager& manager);

private:
    MaterialData m_MaterialData;
    ShaderInstance m_ShaderInstance;
};

} // namespace vrm
