#pragma once

#include "Vroom/Asset/StaticAsset/StaticAsset.h"
#include "Vroom/Asset/AssetInstance/MaterialInstance.h"

#include "Vroom/Render/Abstraction/Shader.h"

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

    [[nodiscard]] const Shader& getShader() const { return m_Shader; }

protected:
    bool loadImpl(const std::string& filePath, AssetManager& manager) override;

private:
    Shader m_Shader;
};

} // namespace vrm
