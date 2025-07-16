#include "Vroom/Asset/StaticAsset/CubemapAsset.h"

#include "Vroom/Asset/Parsing/CubemapParsing.h"
#include "Vroom/Asset/AssetManager.h"

#include <fstream>

using namespace vrm;

CubemapAsset::CubemapAsset()
{

}

CubemapAsset::~CubemapAsset()
{

}

bool CubemapAsset::loadImpl(const std::string& filePath)
{
  std::ifstream ifs;
  ifs.open(filePath);

  VRM_CHECK_RET_FALSE_MSG(ifs.is_open(), "Could not open {}", filePath);

  CubemapData::JsonLayout jdata;

  try
  {
    json j;
    ifs >> j;
    jdata = j;
  }
  catch(const std::exception& e)
  {
    VRM_LOG_ERROR("Could not parse {}. Error: {}", filePath, e.what());
    return false;
  }

  auto& man = AssetManager::Get();

  m_data.posx = man.tryGetAsset<TextureAsset>(jdata.posx);
  VRM_CHECK_RET_FALSE_MSG(m_data.posx.isValid(), "Could not load {} texture", jdata.posx);
  m_data.negx = man.tryGetAsset<TextureAsset>(jdata.negx);
  VRM_CHECK_RET_FALSE_MSG(m_data.negx.isValid(), "Could not load {} texture", jdata.negx);

  m_data.posy = man.tryGetAsset<TextureAsset>(jdata.posy);
  VRM_CHECK_RET_FALSE_MSG(m_data.posy.isValid(), "Could not load {} texture", jdata.posy);
  m_data.negy = man.tryGetAsset<TextureAsset>(jdata.negy);
  VRM_CHECK_RET_FALSE_MSG(m_data.negy.isValid(), "Could not load {} texture", jdata.negy);

  m_data.posz = man.tryGetAsset<TextureAsset>(jdata.posz);
  VRM_CHECK_RET_FALSE_MSG(m_data.posz.isValid(), "Could not load {} texture", jdata.posz);
  m_data.negz = man.tryGetAsset<TextureAsset>(jdata.negz);
  VRM_CHECK_RET_FALSE_MSG(m_data.negz.isValid(), "Could not load {} texture", jdata.negz);
  
  return true;
}
