#include "Vroom/Asset/StaticAsset/ShaderExecAsset.h"

#include "Vroom/Asset/AssetInstance/ShaderExecInstance.h"
#include "Vroom/Core/Assert.h"

#include <nlohmann/json.hpp>
#include <fstream>

using namespace vrm;

VRM_IMPL_STATIC_ASSET(ShaderExec)

bool ShaderExecAsset::loadImpl(const std::string& filePath)
{
  using json = nlohmann::json;

  std::ifstream ifs;
  ifs.open(filePath);
  if (!ifs.is_open())
    return false;
  
  m_ShaderExecData.clear();

  try
  {
    ifs >> m_ShaderExecData;
  }
  catch (const std::exception& e)
  {
    VRM_LOG_ERROR("Error while parsing ShaderExec json data:\n{}", e.what());
    return false;
  }

  return true;
}
