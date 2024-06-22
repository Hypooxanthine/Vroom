#pragma once

#include "Vroom/Asset/StaticAsset/StaticAsset.h"
#include "Vroom/Asset/AssetInstance/ShaderInstance.h"

#include "Vroom/Render/Abstraction/Shader.h"

namespace vrm
{

class ShaderAsset : public StaticAsset
{
public:
    using InstanceType = ShaderInstance;
public:
    ShaderAsset();
    ~ShaderAsset();

    [[nodiscard]] ShaderInstance createInstance();
    
    const Shader& getShader() const;

protected:
    bool loadImpl(const std::string& filePath, AssetManager& manager) override;

private:
    Shader m_Shader;
};

} // namespace vrm
