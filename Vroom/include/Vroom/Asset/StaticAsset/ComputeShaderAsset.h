#pragma once

#include "Vroom/Asset/StaticAsset/StaticAsset.h"
#include "Vroom/Asset/AssetInstance/ComputeShaderInstance.h"

#include "Vroom/Render/Abstraction/ComputeShader.h"

namespace vrm
{

class ComputeShaderAsset : public StaticAsset
{
public:
    using InstanceType = ComputeShaderInstance;

public:
    ComputeShaderAsset();
    ~ComputeShaderAsset();

    [[nodiscard]] ComputeShaderInstance createInstance();

    inline const ComputeShader& getComputeShader() const { return m_ComputeShader; }

protected: 
    bool loadImpl(const std::string& filePath) override;

private:
    ComputeShader m_ComputeShader;
};

} // namespace vrm