#pragma once

#include <string>

#include "AssetManager/MaterialData.h"

namespace vrm
{

class MaterialDataExtractor
{
public:

  MaterialDataExtractor() = default;

  void extract(const MaterialData& data);

public:

  std::string m_typeName = "error";
  std::string m_shadingModelName = "error";

protected:

  std::string _extractType(const MaterialData& data) const;
  std::string _extractShadingModel(const MaterialData& data) const;
};

} // namespace vrm
