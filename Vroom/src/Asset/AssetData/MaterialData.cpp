#include "Vroom/Asset/AssetData/MaterialData.h"

#include "Vroom/Core/Assert.h"

using namespace vrm;
 
MaterialData::MaterialData()
{
}

MaterialData::~MaterialData()
{
}

void MaterialData::setShadingModel(EShadingModel model)
{
  m_shadingModel = model;
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
