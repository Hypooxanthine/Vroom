#include "Vroom/Asset/StaticAsset/MaterialAsset.h"

#include <glm/glm.hpp>
#include <unordered_set>

#include "Vroom/Core/Log.h"

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/Parsing/MaterialParsing.h"

#include "Vroom/Asset/StaticAsset/TextureAsset.h"

using namespace vrm;

MaterialAsset::MaterialAsset()
  : StaticAsset()
{
}

MaterialAsset::~MaterialAsset()
{
}

bool MaterialAsset::loadImpl(const std::string& filePath)
{
  VRM_LOG_INFO("Loading material: {}", filePath);

  auto shadersData = MaterialParsing::Parse(filePath);

  bool compileOk = true;

  if (!m_Shader.addShaderStage(gl::Shader::EShaderType::eVertex, shadersData.vertex, true))
  {
    VRM_LOG_ERROR("Failed to add vertex shader. Error:\n{}", m_Shader.getError());
    compileOk = false;
  }

  if (!m_Shader.addShaderStage(gl::Shader::EShaderType::eFragment, shadersData.fragment, true))
  {
    VRM_LOG_ERROR("Failed to add fragment shader. Error:\n{}", m_Shader.getError());
    compileOk = false;
  }

  if (!m_Shader.validate(true))
  {
    VRM_LOG_ERROR("Failed to validate shader. Error:\n{}", m_Shader.getError());
    compileOk = false;
  }

  VRM_CHECK_RET_FALSE_MSG(compileOk, "Failed to load material: {}", filePath);

  // Loading textures
  for (const std::string& texturePath : shadersData.texturePaths)
  {
    m_Textures.emplace_back(AssetManager::Get().getAsset<TextureAsset>(texturePath));
  }

  return true;
}
