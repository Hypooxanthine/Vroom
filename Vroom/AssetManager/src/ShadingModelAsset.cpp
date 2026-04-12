#include "AssetManager/ShadingModelAsset.h"

#include <fstream>
#include <unordered_map>

#include "AssetManager/AssetManager.h"
#include "AssetManager/ShaderAsset.h"
#include "AssetManager/Json.h"
#include "AssetManager/ShaderParsing.h"

using namespace vrm;

ShadingModelAsset::ShadingModelAsset()
{
}

ShadingModelAsset::~ShadingModelAsset()
{
}

bool ShadingModelAsset::loadImpl(const std::string &filePath)
{
  AssetManager &manager = AssetManager::Get();

  VRM_LOG_TRACE("Loading shading model: {}", filePath);

  // Base shading model material
  {
    static const std::string baseShaderPath = "Resources/Engine/Shader/ShadingModel/ShadingModelBase_Shader.json";
    VRM_CHECK_RET_FALSE_MSG(manager.tryLoadAsset<ShaderAsset>(baseShaderPath), "Could not load shading model base shader: {}", baseShaderPath);

    ShaderAsset::Handle baseShader = manager.getAsset<ShaderAsset>(baseShaderPath);
    m_data = ShaderData{baseShader->getShaderData()};
  }

  // Implementation of the shading model
  {
    VRM_CHECK_RET_FALSE_MSG(manager.tryLoadAsset<ShaderAsset>(filePath), "Could not load material specific shader: {}", filePath);

    ShaderAsset::Handle specificShader = manager.getAsset<ShaderAsset>(filePath);
    m_data.absorb(specificShader->getShaderData());
  }

  return true;
}
