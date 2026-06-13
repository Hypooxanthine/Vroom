#include "AssetManager/MaterialData.h"
#include <set>

#include "Core/Assert.h"


using namespace vrm;

static const std::set<MaterialData::EType> s_customShaderSupportedTypes = { MaterialData::EType::eCustomShader,
                                                                            MaterialData::EType::ePostProcess };

MaterialData::MaterialData()
{}

MaterialData::~MaterialData()
{}

void MaterialData::setType(EType type)
{
  if (m_type == type)
    return;

  m_type         = type;
  m_shadingModel = EShadingModel::eNone;
  m_customShader = "";
}

MaterialData::EType MaterialData::getType() const
{
  return m_type;
}

void MaterialData::setShadingModel(EShadingModel model)
{
  VRM_ASSERT_MSG(m_type == EType::eShadingModel, "Type is not shading model");
  m_shadingModel = model;
}

void MaterialData::setCustomShader(const std::string& customShader)
{
  VRM_ASSERT_MSG(s_customShaderSupportedTypes.contains(m_type), "Material type does not support custom shader");
  m_customShader = customShader;
}

void MaterialData::setParameter(const Parameter& param)
{
  VRM_CHECK_RET_MSG(param.type != Parameter::Type::eNone, "Parameter must have a type");

  if (param.type == Parameter::eSampler2D)
  {
    if (!m_parameters.contains(param.name) || m_parameters.at(param.name).type != Parameter::eSampler2D)
      ++m_textureCount;
  }
  else
  {
    if (m_parameters.contains(param.name) && m_parameters.at(param.name).type == Parameter::eSampler2D)
      --m_textureCount;
  }

  m_parameters[param.name] = param;
}

bool MaterialData::hasParameter(const std::string& name) const
{
  return m_parameters.contains(name);
}

const MaterialData::Parameter& MaterialData::getParameter(const std::string& name) const
{
  VRM_DEBUG_ASSERT_MSG(hasParameter(name), "Unknown parameter");

  return m_parameters.at(name);
}
