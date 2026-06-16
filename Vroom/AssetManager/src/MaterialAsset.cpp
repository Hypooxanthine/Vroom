#include "AssetManager/MaterialAsset.h"
#include <filesystem>
#include <unordered_map>

#include "AssetManager/AssetManager.h"
#include "AssetManager/JsonFile.h"
#include "AssetManager/MaterialParsing.h"
#include "AssetManager/ShaderAsset.h"
#include "AssetManager/ShadingModelAsset.h"
#include "Core/Assert.h"
#include "Core/Log.h"
#include "Tools/Utility.h"

using namespace vrm;

static const std::unordered_map<MaterialData::EShadingModel, std::string> S_SHADING_MODEL_SPECIFIC_SHADERS = {
  { MaterialData::EShadingModel::ePhong, "Resources/Engine/Shader/ShadingModel/ShadingModel_Phong.json" },
  {   MaterialData::EShadingModel::ePBR,   "Resources/Engine/Shader/ShadingModel/ShadingModel_PBR.json" },
};

std::string S_POST_PROCESS_BASE_SHADER = "Resources/Engine/Shader/RenderFullscreen/RenderFullscreen_shader.json";

MaterialAsset::MaterialAsset() : StaticAsset()
{}

MaterialAsset::~MaterialAsset()
{}

void MaterialAsset::applyUniforms(const gl::Shader& shader) const
{
  shader.bind();

  uint8_t textureCount = 0;
  for (const auto& [pName, param] : m_data.getParameters())
  {
    auto visitor = overloaded{
      [&, this](float value)
      {
        shader.setUniform1f(pName, value);
      },
      [&, this](const glm::vec2& value)
      {
        shader.setUniform2f(pName, value);
      },
      [&, this](const glm::vec3& value)
      {
        shader.setUniform3f(pName, value);
      },
      [&, this](const glm::vec4& value)
      {
        shader.setUniform4f(pName, value);
      },
      [&, this](const glm::mat4& value)
      {
        shader.setUniformMat4f(pName, value);
      },
      [&, this](const MaterialData::TextureData& value)
      {
      },
    };

    std::visit(visitor, param.value);

    if (param.type == MaterialData::Parameter::eSampler2D)
    {
      shader.setTexture(pName, m_textures.at(textureCount)->getGPUTexture(), textureCount);
      ++textureCount;
    }
  }
}

bool MaterialAsset::buildShaderData()
{
  AssetManager& manager = AssetManager::Get();

  // Settings defines depending on material parameters
  {
    ShaderData::Define d = { .name = "VRM_TEXTURE_COUNT", .value = std::to_string(m_data.getTextureCount()) };
    m_materialShaderData.addDefine(ShaderData::EShaderType::eAll, d);

    if (m_data.getTextureCount() > 0)
    {
      d = { .name = "VRM_ENABLE_TEXTURES" };

      m_materialShaderData.addDefine(ShaderData::EShaderType::eAll, d);
    }

    for (const auto& [pName, param] : m_data.getParameters())
    {
      if (param.type == MaterialData::Parameter::eSampler2D)
      {
        d = { .name = "VRM_TEXTURE_" + pName };
        m_materialShaderData.addDefine(ShaderData::EShaderType::eAll, d);

        const MaterialData::TextureData& texParam = param.getValue<MaterialData::TextureData>();
        std::filesystem::path            texPath  = applyPathOrder(texParam.resourceName);

        VRM_CHECK_RET_FALSE_MSG(manager.tryLoadAsset<TextureAsset>(texPath), "Could not load material texture: {}",
                                texPath.string());

        m_textures.push_back(manager.getAsset<TextureAsset>(texPath));
      }
      else
      {
        d = { .name = "VRM_UNIFORM_" + pName };
        m_materialShaderData.addDefine(ShaderData::EShaderType::eAll, d);
      }
    }
  }

  return true;
}

bool MaterialAsset::loadImpl(const std::filesystem::path& filePath)
{
  VRM_LOG_TRACE("Loading material: {}", filePath.string());

  nlohmann::json j;
  VRM_CHECK_RET_FALSE_MSG(ReadJsonFile(filePath, j), "Could not read material file: {}", filePath.string());

  MaterialData data;
  VRM_CHECK_RET_FALSE_MSG(MaterialParsing::Parse(j, data), "Could not parse MaterialData from file: {}",
                          filePath.string());

  return buildFromData(data);
}

bool MaterialAsset::loadFromData(const MaterialData& data, const std::filesystem::path& sourcePath)
{
  // Resolve relative resources (e.g. textures) relative to where the data came
  // from, exactly as a file-based load would via applyPathOrder().
  setFilePath(sourcePath);

  VRM_CHECK_RET_FALSE_MSG(buildFromData(data), "Could not build material from in-memory data");

  // load() bumps the generation for us on the file path; do the same here so
  // consumers caching by generation rebuild from the new data.
  incrementGeneration();
  return true;
}

bool MaterialAsset::buildFromData(const MaterialData& data)
{
  m_data = data;
  // Reset derived state so a rebuild (reload / preview) doesn't accumulate.
  m_textures.clear();

  AssetManager& manager = AssetManager::Get();

  // Base material shader
  {
    static const std::string baseShaderPath = "Resources/Engine/Shader/MaterialBase_Shader.json";
    VRM_CHECK_RET_FALSE_MSG(manager.tryLoadAsset<ShaderAsset>(baseShaderPath),
                            "Could not load material base shader: {}", baseShaderPath);

    ShaderAsset::Handle baseShader = manager.getAsset<ShaderAsset>(baseShaderPath);
    m_materialShaderData           = ShaderData{ baseShader->getShaderData() };
  }

  // Shading model
  if (m_data.getType() == MaterialData::EType::eShadingModel)
  {
    auto findIt = S_SHADING_MODEL_SPECIFIC_SHADERS.find(m_data.getShadingModel());
    VRM_CHECK_RET_FALSE_MSG(findIt != S_SHADING_MODEL_SPECIFIC_SHADERS.end(), "Shading model {} is not supported",
                            m_data.getShadingModelName());

    const std::string& ID = findIt->second;
    VRM_CHECK_RET_FALSE_MSG(manager.tryLoadAsset<ShadingModelAsset>(ID), "Couldn't load {} shading model at path {}",
                            m_data.getShadingModelName(), ID);

    m_shadingModel = manager.getAsset<ShadingModelAsset>(ID);
    m_materialShaderData.absorb(m_shadingModel->getData());
    m_materialShaderData.addDefine(ShaderData::EShaderType::eAll, { "VRM_SHADING_MODEL", "1" });
  }
  // Post process material
  else if (m_data.getType() == MaterialData::EType::ePostProcess)
  {
    // Base post process data
    {
      ShaderAsset::Handle shader = manager.tryGetAsset<ShaderAsset>(S_POST_PROCESS_BASE_SHADER);
      VRM_CHECK_RET_FALSE_MSG(shader.isValid(), "Could not load engine post process base material {}",
                              S_POST_PROCESS_BASE_SHADER);
      m_materialShaderData.absorb(shader->getShaderData());
    }
    // Post process implementation
    {
      ShaderAsset::Handle shader = manager.tryGetAsset<ShaderAsset>(m_data.getCustomShader());
      VRM_CHECK_RET_FALSE_MSG(shader.isValid(), "Could not load post process material implementation {}",
                              m_data.getCustomShader());
      m_materialShaderData.absorb(shader->getShaderData());
    }

    m_materialShaderData.addDefine(ShaderData::EShaderType::eAll, { "VRM_POST_PROCESS_MATERIAL", "1" });
  }
  // Custom shaders material
  else if (m_data.getType() == MaterialData::EType::eCustomShader)
  {
    ShaderAsset::Handle shader = manager.tryGetAsset<ShaderAsset>(m_data.getCustomShader());
    VRM_CHECK_RET_FALSE_MSG(shader.isValid(), "Could not load custom material implementation {}",
                            m_data.getCustomShader());

    m_materialShaderData.absorb(shader->getShaderData());
    m_materialShaderData.addDefine(ShaderData::EShaderType::eAll, { "VRM_CUSTOM_SHADER", "1" });
  }
  else
  {
    VRM_LOG_ERROR("Material type {} is not supported", (size_t)m_data.getType());
  }

  VRM_CHECK_RET_FALSE_MSG(buildShaderData(), "Could not build material shader data");

  return true;
}
