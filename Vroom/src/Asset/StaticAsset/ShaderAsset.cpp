#include "Vroom/Asset/StaticAsset/ShaderAsset.h"

#include "Vroom/Core/Assert.h"
#include "Vroom/Asset/Parsing/ShaderParsing.h"

#include <nlohmann/json.hpp>
#include <fstream>

using namespace vrm;

bool ShaderAsset::loadImpl(const std::string& filePath)
{
  using json = nlohmann::json;

  VRM_LOG_TRACE("Loading shader: {}", filePath);

  std::ifstream ifs;
  ifs.open(filePath);
  if (!ifs.is_open())
    return false;

  json j;

  try
  {
    ifs >> j;
  }
  catch (const std::exception& e)
  {
    VRM_LOG_ERROR("Error while parsing ShaderExec json data:\n{}", e.what());
    return false;
  }

  return ShaderParsing::Parse(j, m_data);
}