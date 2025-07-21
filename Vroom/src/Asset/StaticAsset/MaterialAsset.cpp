#include "Vroom/Asset/StaticAsset/MaterialAsset.h"

#include <fstream>
#include <unordered_map>

#include "Vroom/Core/Assert.h"
#include "Vroom/Asset/Parsing/MaterialParsing.h"

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/StaticAsset/ShadingModelAsset.h"
#include "Vroom/Asset/StaticAsset/ShaderAsset.h"

using namespace vrm;

static const std::unordered_map<MaterialData::EShadingModel, std::string> S_SHADING_MODEL_SPECIFIC_SHADERS =
{
  { MaterialData::EShadingModel::ePhong, "Resources/Engine/Shader/ShadingModel/ShadingModel_Phong.json" },
  { MaterialData::EShadingModel::ePBR, "Resources/Engine/Shader/ShadingModel/ShadingModel_PBR.json" },
};

std::string S_POST_PROCESS_BASE_SHADER = "Resources/Engine/Shader/RenderFullscreen/RenderFullscreen_shader.json";

MaterialAsset::MaterialAsset()
    : StaticAsset()
{
}

MaterialAsset::~MaterialAsset()
{
}

// From https://en.cppreference.com/w/cpp/utility/variant/visit2
template <class... Ts>
struct overloaded : Ts...
{
  using Ts::operator()...;
};
// explicit deduction guide (not needed as of C++20)
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

void MaterialAsset::applyUniforms(const gl::Shader& shader) const
{
  shader.bind();

  uint8_t textureCount = 0;
  for (const auto &[pName, param] : m_data.getParameters())
  {
    auto visitor = overloaded{
      [&, this](float value) { shader.setUniform1f(pName, value); },
      [&, this](const glm::vec2& value) { shader.setUniform2f(pName, value); },
      [&, this](const glm::vec3& value) { shader.setUniform3f(pName, value); },
      [&, this](const glm::vec4& value) { shader.setUniform4f(pName, value); },
      [&, this](const glm::mat4& value) { shader.setUniformMat4f(pName, value); },
      [&, this](const std::string& value) { },
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
  AssetManager &manager = AssetManager::Get();

  // Settings defines depending on material parameters
  {
    ShaderData::Define d =
    {
      .name = "VRM_TEXTURE_COUNT",
      .value = std::to_string(m_data.getTextureCount())
    };
    m_materialShaderData.addDefine(ShaderData::EShaderType::eAll, d);

    if (m_data.getTextureCount() > 0)
    {
      d = {.name = "VRM_ENABLE_TEXTURES"};

      m_materialShaderData.addDefine(ShaderData::EShaderType::eAll, d);
    }

    for (const auto &[pName, param] : m_data.getParameters())
    {
      if (param.type == MaterialData::Parameter::eSampler2D)
      {
        d = {.name = "VRM_TEXTURE_" + pName };
        m_materialShaderData.addDefine(ShaderData::EShaderType::eAll, d);

        std::string texturePath = applyPathOrder(param.getValue<std::string>());

        VRM_CHECK_RET_FALSE_MSG(manager.tryLoadAsset<TextureAsset>(texturePath), "Could not load material texture: {}", texturePath);

        m_textures.push_back(manager.getAsset<TextureAsset>(texturePath));
      }
      else
      {
        d = {.name = "VRM_UNIFORM_" + pName };
        m_materialShaderData.addDefine(ShaderData::EShaderType::eAll, d);
      }
    }
  }

  return true;
}

bool MaterialAsset::loadImpl(const std::string &filePath)
{
  using json = nlohmann::json;

  VRM_LOG_TRACE("Loading material: {}", filePath);

  std::ifstream ifs;
  ifs.open(filePath);
  VRM_CHECK_RET_FALSE_MSG(ifs.is_open(), "Could not open file: {}", filePath);

  json j;

  try
  {
    ifs >> j;
  }
  catch (const std::exception &e)
  {
    VRM_LOG_ERROR("Json data from file \"{}\" could not be read. Parser error:\n{}", filePath, e.what());
    return false;
  }

  VRM_CHECK_RET_FALSE_MSG(MaterialParsing::Parse(j, m_data), "Could not parse MaterialData from file: {}", filePath);

  AssetManager& manager = AssetManager::Get();

  // Base material shader
  {
    static const std::string baseShaderPath = "Resources/Engine/Shader/MaterialBase_Shader.json";
    VRM_CHECK_RET_FALSE_MSG(manager.tryLoadAsset<ShaderAsset>(baseShaderPath), "Could not load material base shader: {}", baseShaderPath);

    ShaderAsset::Handle baseShader = manager.getAsset<ShaderAsset>(baseShaderPath);
    m_materialShaderData = ShaderData{baseShader->getShaderData()};
  }

  // Shading model
  if (m_data.getShadingModel() != MaterialData::EShadingModel::eNone)
  {
    const std::string &ID = S_SHADING_MODEL_SPECIFIC_SHADERS.at(m_data.getShadingModel());
    VRM_CHECK_RET_FALSE_MSG(manager.tryLoadAsset<ShadingModelAsset>(ID), "Couldn't load {} shading model at path {}", m_data.getShadingModelName(), ID);

    ShadingModelAsset::Handle shadingModel = manager.getAsset<ShadingModelAsset>(ID);
    m_materialShaderData.absorb(shadingModel->getData());
    m_materialShaderData.addDefine(ShaderData::EShaderType::eAll, { "VRM_SHADING_MODEL", "1" });
  }
  // Psot process material
  else if (m_data.getType() == MaterialData::EType::ePostProcess)
  {
    // Base post process data
    {
      ShaderAsset::Handle shader = manager.tryGetAsset<ShaderAsset>(S_POST_PROCESS_BASE_SHADER);
      VRM_CHECK_RET_FALSE_MSG(shader.isValid(), "Could not load engine post process base material {}", S_POST_PROCESS_BASE_SHADER);
      m_materialShaderData.absorb(shader->getShaderData());
    }
    // Post process implementation
    {
      ShaderAsset::Handle shader = manager.tryGetAsset<ShaderAsset>(m_data.getCustomShader());
      VRM_CHECK_RET_FALSE_MSG(shader.isValid(), "Could not load post process material implementation {}", m_data.getCustomShader());
      m_materialShaderData.absorb(shader->getShaderData());
    }

    m_materialShaderData.addDefine(ShaderData::EShaderType::eAll, { "VRM_POST_PROCESS_MATERIAL", "1" });
  }
  // Custom shaders material
  else if (m_data.getType() == MaterialData::EType::eCustomShader)
  {
    ShaderAsset::Handle shader = manager.tryGetAsset<ShaderAsset>(m_data.getCustomShader());
    m_materialShaderData.absorb(shader->getShaderData());
    m_materialShaderData.addDefine(ShaderData::EShaderType::eAll, { "VRM_CUSTOM_SHADER", "1" });
  }

  VRM_CHECK_RET_FALSE_MSG(buildShaderData(), "Could not build material shader data");

  return true;
}
