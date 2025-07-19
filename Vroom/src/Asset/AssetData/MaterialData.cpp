#include "Vroom/Asset/AssetData/MaterialData.h"

#include "Vroom/Core/Assert.h"

using namespace vrm;
 
MaterialData::MaterialData()
{
}

MaterialData::~MaterialData()
{
}

void MaterialData::setType(EType type)
{
  if (m_type == type) return;
  
  m_type = type;
  m_shadingModel = EShadingModel::eNone;
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
  VRM_ASSERT_MSG(m_type == EType::eCustomShader, "Type is not custom shader");
  m_customShader = customShader;
}

void MaterialData::addParameter(const Parameter& param)
{
  VRM_CHECK_RET_MSG(param.type != Parameter::Type::eNone, "Parameter must have a type");
  VRM_CHECK_RET_MSG(!m_parameters.contains(param.name), "Material parameter already set !");

  if (param.type == Parameter::eSampler2D)
    ++m_textureCount;
  
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
