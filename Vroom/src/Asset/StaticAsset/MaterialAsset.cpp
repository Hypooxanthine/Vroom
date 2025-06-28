#include "Vroom/Asset/StaticAsset/MaterialAsset.h"

#include <fstream>
#include <unordered_map>

#include "Vroom/Core/Assert.h"
#include "Vroom/Asset/Parsing/MaterialParsing.h"

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/StaticAsset/ShadingModelAsset.h"

using namespace vrm;

static const std::unordered_map<MaterialData::EShadingModel, std::string> S_SHADING_MODEL_SPECIFIC_SHADERS =
{
  { MaterialData::EShadingModel::ePhong, "Resources/Engine/Shader/ShadingModel/ShadingModel_Phong.json" }
};

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

  std::string prefix = "u_" + m_data.getShadingModelName();
  uint8_t textureCount = 0;
  for (const auto &[name, param] : m_data.getParameters())
  {
    auto visitor = overloaded{
      [&, this](float value) { shader.setUniform1f(prefix + name, value); },
      [&, this](const glm::vec2& value) { shader.setUniform2f(prefix + name, value); },
      [&, this](const glm::vec3& value) { shader.setUniform3f(prefix + name, value); },
      [&, this](const glm::vec4& value) { shader.setUniform4f(prefix + name, value); },
      [&, this](const std::string& textureName) { shader.setTexture(prefix + "Textures", m_textures.at(textureCount)->getGPUTexture(), textureCount); ++textureCount; }
    };

    std::visit(visitor, param.value);
  }

  if (m_data.getTextureCount() > 0)
  {
    std::vector<int> slots(m_data.getTextureCount());
    std::iota(slots.begin(), slots.end(), 0);
    shader.setUniform1iv(prefix + "Textures", m_data.getTextureCount(), slots.data());
  }
}

bool MaterialAsset::buildShaderData()
{
  AssetManager &manager = AssetManager::Get();

  // Settings defines depending on material parameters
  {
    std::string prefix = "VRM_" + m_data.getShadingModelName();

    ShaderData::Define d =
    {
      .name = prefix + "_TEXTURES_COUNT",
      .value = std::to_string(m_data.getTextureCount())
    };
    m_materialShaderData.addDefine(ShaderData::EShaderType::eFragment, d);

    if (m_data.getTextureCount() > 0)
    {
      d = {.name = prefix + "_ENABLE_TEXTURES"};

      m_materialShaderData.addDefine(ShaderData::EShaderType::eFragment, d);
    }

    for (const auto &[name, param] : m_data.getParameters())
    {
      std::string paramPrefix = prefix + "_" + name;

      if (param.isTexture())
      {
        d = {.name = paramPrefix + "_TEXTURE"};
        m_materialShaderData.addDefine(ShaderData::EShaderType::eFragment, d);
        d = {.name = paramPrefix + "_TEXTURE_SLOT", .value = std::to_string(m_textures.size())};
        m_materialShaderData.addDefine(ShaderData::EShaderType::eFragment, d);

        VRM_CHECK_RET_FALSE_MSG(manager.tryLoadAsset<TextureAsset>(param.getTexture()), "Could not load material texture: {}", param.getTexture());

        m_textures.push_back(manager.getAsset<TextureAsset>(param.getTexture()));
      }
      else
      {
        d = {.name = paramPrefix + "_UNIFORM"};
        m_materialShaderData.addDefine(ShaderData::EShaderType::eFragment, d);
      }
    }
  }

  // Last defines...
  m_materialShaderData.addDefine(ShaderData::EShaderType::eAll, {.name = "VRM_CLUSTERED_RENDERING"});

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
    VRM_CHECK_FAIL_MSG("Json data from file \"{}\" could not be read. Parser error:\n{}", filePath, e.what());
  }

  VRM_CHECK_RET_FALSE_MSG(MaterialParsing::Parse(j, m_data), "Could not parse MaterialData from file: {}", filePath);

  AssetManager &manager = AssetManager::Get();

  // Loading shading model
  {
    const std::string &ID = S_SHADING_MODEL_SPECIFIC_SHADERS.at(m_data.getShadingModel());
    VRM_CHECK_RET_FALSE_MSG(manager.tryLoadAsset<ShadingModelAsset>(ID), "Couldn't load {} shading model at path {}", m_data.getShadingModelName(), ID);

    ShadingModelAsset::Handle shadingModel = manager.getAsset<ShadingModelAsset>(ID);
    m_materialShaderData = ShaderData{shadingModel->getData()};
  }

  VRM_CHECK_RET_FALSE_MSG(buildShaderData(), "Could not build material shader data");

  return true;
}
