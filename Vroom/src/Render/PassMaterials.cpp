#include "Vroom/Render/PassMaterials.h"

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Render/MaterialDefines.h"

using namespace vrm;

// PassMaterial

PassMaterial::PassMaterial()
{
}

PassMaterial::~PassMaterial()
{
}

void PassMaterial::setMaterialAsset(MaterialAsset::Handle material)
{
  m_materialAsset = material;
  m_needsPrepare = true;
}

bool PassMaterial::prepare(const MaterialDefines &defines)
{
  AssetManager &manager = AssetManager::Get();

  ShaderData shaderData = m_materialAsset->getShaderData();

  for (const auto& [define, value] : defines)
  {
    shaderData.addDefine(ShaderData::EShaderType::eAll, ShaderData::Define{ define, value });
  }

  ShaderData::FullShader rawShaderData = shaderData.combine();
  VRM_CHECK_RET_FALSE_MSG(
      m_gpuShader.addShaderStage(gl::Shader::EShaderType::eVertex, rawShaderData.vertex.sourceCode, true),
      "With shader:\n{}\n\nCould not compile material vertex shader. Error log:\n{}", rawShaderData.vertex.sourceCode, m_gpuShader.getError());
  VRM_CHECK_RET_FALSE_MSG(
      m_gpuShader.addShaderStage(gl::Shader::EShaderType::eFragment, rawShaderData.fragment.sourceCode, true),
      "With shader:\n{}\n\nCould not compile material fragment shader. Error log:\n{}", rawShaderData.fragment.sourceCode, m_gpuShader.getError());

  VRM_ASSERT_MSG(
      m_gpuShader.validate(true),
      "Could not validate material shader. Error log:\n{}", m_gpuShader.getError());

  // std::string dumpPath = getFilePath() + ".dump.glsl";
  // rawShaderData.fragment.dump(dumpPath);
  // VRM_LOG_INFO("Fragment dumped in {}", dumpPath);

  m_needsPrepare = false;
  return true;
}

// PassMaterials

PassMaterials::PassMaterials()
{

}

PassMaterials::~PassMaterials()
{

}

const PassMaterial& PassMaterials::getMaterial(const Key& key)
{
  auto [it, inserted] = m_materials.try_emplace(key);
  PassMaterial& material = it->second;

  if (inserted)
  {
    material.setMaterialAsset(key.asset);
    if (key.defines)
      material.prepare(*key.defines);
    else
      material.prepare({});
  }
  
  return material;
}

void PassMaterials::releaseMaterial(const Key& key)
{
  m_materials.erase(key);
}

void PassMaterials::clear()
{
  m_materials.clear();
}
