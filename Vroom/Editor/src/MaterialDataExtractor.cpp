#include "Editor/MaterialDataExtractor.h"
#include <unordered_map>

#include "AssetManager/MaterialData.h"

using namespace vrm;

void MaterialDataExtractor::extract(const MaterialData& data)
{

  m_typeName = _extractType(data);
  m_shadingModelName = _extractShadingModel(data);
}

std::string MaterialDataExtractor::_extractType(const MaterialData& data) const
{
  static std::unordered_map<MaterialData::EType, std::string> convTable = {
    { MaterialData::EType::eShadingModel, "Shading model" },
    {  MaterialData::EType::ePostProcess,  "Post process" },
    { MaterialData::EType::eCustomShader, "Custom shader" },
    {    MaterialData::EType::eUndefined,     "Undefined" },
  };

  return convTable.at(data.getType());
}

std::string MaterialDataExtractor::_extractShadingModel(const MaterialData& data) const
{
  static std::unordered_map<MaterialData::EShadingModel, std::string> convTable = {
    { MaterialData::EShadingModel::ePhong, "Phong shading" },
    {   MaterialData::EShadingModel::ePBR,   "PBR shading" },
    {  MaterialData::EShadingModel::eNone,          "None" },
  };

  return convTable.at(data.getShadingModel());
}
