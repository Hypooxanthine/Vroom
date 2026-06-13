#include "AssetManager/ShadingModelAsset.h"

#include "AssetManager/AssetManager.h"
#include "AssetManager/ShaderAsset.h"

using namespace vrm;

ShadingModelAsset::ShadingModelAsset()
{}

ShadingModelAsset::~ShadingModelAsset()
{}

bool ShadingModelAsset::loadImpl(const std::filesystem::path& filePath)
{
  AssetManager& manager = AssetManager::Get();

  VRM_LOG_TRACE("Loading shading model: {}", filePath.string());

  // Base shading model material
  {
    static const std::string baseShaderPath = "Resources/Engine/Shader/ShadingModel/ShadingModelBase_Shader.json";
    VRM_CHECK_RET_FALSE_MSG(manager.tryLoadAsset<ShaderAsset>(baseShaderPath),
                            "Could not load shading model base shader: {}", baseShaderPath);

    ShaderAsset::Handle baseShader = manager.getAsset<ShaderAsset>(baseShaderPath);
    m_data                         = ShaderData{ baseShader->getShaderData() };
  }

  // Implementation of the shading model
  {
    // The implementation shader of the shading model must be the name of the shading model with a "Shader" suffix.
    std::filesystem::path shaderPath = filePath;
    shaderPath.replace_extension();
    shaderPath += "Shader";
    shaderPath += filePath.extension();

    VRM_CHECK_RET_FALSE_MSG(manager.tryLoadAsset<ShaderAsset>(shaderPath),
                            "Could not load material specific shader: {}", shaderPath.string());

    ShaderAsset::Handle specificShader = manager.getAsset<ShaderAsset>(shaderPath);
    m_data.absorb(specificShader->getShaderData());
  }

  return true;
}
