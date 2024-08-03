#include "Vroom/Asset/StaticAsset/ComputeShaderAsset.h"

namespace vrm
{

ComputeShaderAsset::ComputeShaderAsset()
    : StaticAsset()
{
}

ComputeShaderAsset::~ComputeShaderAsset()
{
}

ComputeShaderInstance ComputeShaderAsset::createInstance()
{
    return ComputeShaderInstance(this);
}

bool ComputeShaderAsset::loadImpl(const std::string& filePath)
{
    return m_ComputeShader.loadFromFile(filePath);
}

} // namespace vrm