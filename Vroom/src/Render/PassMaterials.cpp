#include "Vroom/Render/PassMaterials.h"

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Render/MaterialDefines.h"

using namespace vrm;

// PassMaterial

PassMaterial::PassMaterial() {}

PassMaterial::~PassMaterial() {}

void PassMaterial::setMaterialAsset(MaterialAsset::Handle material)
{
  m_materialAsset = material;
  m_gpuShader.unload();
  m_needsPrepare = true;
}

bool PassMaterial::prepare(const MaterialDefines& defines)
{
  AssetManager& manager = AssetManager::Get();

  ShaderData shaderData = m_materialAsset->getShaderData();

  for (const auto& [define, value] : defines)
  {
    shaderData.addDefine(ShaderData::EShaderType::eAll,
                         ShaderData::Define{ define, value });
  }

  ShaderData::FullShader rawShaderData = shaderData.combine();

  static const std::unordered_map<ShaderData::EShaderType,
                                  gl::Shader::EShaderType>
    s_convertTable = {
      {   ShaderData::EShaderType::eVertex,gl::Shader::EShaderType::eVertex                                           },
      { ShaderData::EShaderType::eGeometry,
       gl::Shader::EShaderType::eGeometry                                    },
      { ShaderData::EShaderType::eFragment,
       gl::Shader::EShaderType::eFragment                                    },
      {  ShaderData::EShaderType::eCompute, gl::Shader::EShaderType::eCompute },
  };

  for (size_t i = 0; i < (size_t)ShaderData::EShaderType::eCount; ++i)
  {
    auto               stage = ShaderData::EShaderType(i);
    const std::string& src   = rawShaderData[stage];
    if (src.size() > 0)
    {
      VRM_CHECK_RET_FALSE_MSG(
        m_gpuShader.addShaderStage(s_convertTable.at(stage), src, true),
        "With shader:\n{}\n\nCould not compile material shader. Error log:\n{}",
        src, m_gpuShader.getError());
    }
  }

  VRM_ASSERT_MSG(m_gpuShader.validate(true),
                 "Could not validate material shader. Error log:\n{}",
                 m_gpuShader.getError());

  // std::string dumpPath = getFilePath() + ".dump.glsl";
  // rawShaderData.fragment.dump(dumpPath);
  // VRM_LOG_INFO("Fragment dumped in {}", dumpPath);

  m_needsPrepare = false;
  return true;
}

// PassMaterials

PassMaterials::PassMaterials() {}

PassMaterials::~PassMaterials() {}

const PassMaterial& PassMaterials::getMaterial(const Key& key)
{
  auto [it, inserted]    = m_materials.try_emplace(key);
  PassMaterial& material = it->second;

  if (inserted)
  {
    material.setMaterialAsset(key.asset);
    if (key.defines) material.prepare(*key.defines);
    else material.prepare({});
  }

  return material;
}

void PassMaterials::releaseMaterial(const Key& key) { m_materials.erase(key); }

void PassMaterials::clear() { m_materials.clear(); }
