#include "Vroom/Asset/AssetData/MaterialData.h"

#include "Vroom/Render/Abstraction/Shader.h"

using namespace vrm;

MaterialData::MaterialData()
{
}

MaterialData::~MaterialData()
{
}

void MaterialData::applyUniforms(const gl::Shader &shader) const
{
  shader.bind();

  for (const auto &[name, value] : m_Mat4Uniforms)
    shader.setUniformMat4f(name, value);
  for (const auto &[name, value] : m_Vec4Uniforms)
    shader.setUniform4f(name, value.x, value.y, value.z, value.w);
  for (const auto &[name, value] : m_Vec3Uniforms)
    shader.setUniform3f(name, value.x, value.y, value.z);
  for (const auto &[name, value] : m_Vec2Uniforms)
    shader.setUniform2f(name, value.x, value.y);
  for (const auto &[name, value] : m_FloatUniforms)
    shader.setUniform1f(name, value);
  for (const auto &[name, value] : m_IntUniforms)
    shader.setUniform1i(name, value);
  for (const auto &[name, value] : m_BoolUniforms)
    shader.setUniform1i(name, value);
  for (const auto &[name, value] : m_TextureUniforms)
    shader.setUniform1ui(name, value);
}

void MaterialData::setMat4Uniform(const std::string &name, const glm::mat4 &value)
{
  m_Mat4Uniforms[name] = value;
}

void MaterialData::setVec4Uniform(const std::string &name, const glm::vec4 &value)
{
  m_Vec4Uniforms[name] = value;
}

void MaterialData::setVec3Uniform(const std::string &name, const glm::vec3 &value)
{
  m_Vec3Uniforms[name] = value;
}

void MaterialData::setVec2Uniform(const std::string &name, const glm::vec2 &value)
{
  m_Vec2Uniforms[name] = value;
}

void MaterialData::setFloatUniform(const std::string &name, float value)
{
  m_FloatUniforms[name] = value;
}

void MaterialData::setIntUniform(const std::string &name, int value)
{
  m_IntUniforms[name] = value;
}

void MaterialData::setBoolUniform(const std::string &name, bool value)
{
  m_BoolUniforms[name] = value;
}

void MaterialData::setTextureUniform(const std::string &name, unsigned int value)
{
  m_TextureUniforms[name] = value;
}
