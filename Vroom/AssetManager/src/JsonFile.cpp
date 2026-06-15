#include "AssetManager/JsonFile.h"

#include <fstream>

#include "Core/Log.h"

using namespace vrm;

bool vrm::ReadJsonFile(const std::filesystem::path& path, nlohmann::json& out)
{
  std::ifstream ifs(path);
  if (!ifs.is_open())
  {
    VRM_LOG_ERROR("Could not open file for reading: {}", path.string());
    return false;
  }

  try
  {
    out = nlohmann::json::parse(ifs);
  } catch (const std::exception& e)
  {
    VRM_LOG_ERROR("Could not parse JSON from file \"{}\". Parser error:\n{}", path.string(), e.what());
    return false;
  }

  return true;
}

bool vrm::WriteJsonFile(const std::filesystem::path& path, const nlohmann::json& j, int indent)
{
  std::ofstream ofs(path, std::ofstream::trunc);
  if (!ofs.is_open())
  {
    VRM_LOG_ERROR("Could not open file for writing: {}", path.string());
    return false;
  }

  try
  {
    ofs << j.dump(indent);
  } catch (const std::exception& e)
  {
    VRM_LOG_ERROR("Could not serialize JSON to file \"{}\". Error:\n{}", path.string(), e.what());
    return false;
  }

  return true;
}
