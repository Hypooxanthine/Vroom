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

  VRM_CHECK_RET_FALSE_MSG(_loadTexture(jdata.posx, m_data.posx), "Could not load {} texture", jdata.posx);
  VRM_CHECK_RET_FALSE_MSG(_loadTexture(jdata.negx, m_data.negx), "Could not load {} texture", jdata.negx);

  VRM_CHECK_RET_FALSE_MSG(_loadTexture(jdata.posy, m_data.posy), "Could not load {} texture", jdata.posy);
  VRM_CHECK_RET_FALSE_MSG(_loadTexture(jdata.negy, m_data.negy), "Could not load {} texture", jdata.negy);
  
  VRM_CHECK_RET_FALSE_MSG(_loadTexture(jdata.posz, m_data.posz), "Could not load {} texture", jdata.posz);
  VRM_CHECK_RET_FALSE_MSG(_loadTexture(jdata.negz, m_data.negz), "Could not load {} texture", jdata.negz);
  
  return true;
}

bool CubemapAsset::_loadTexture(const std::string& filePath, ByteTextureData& textureData)
{
  VRM_LOG_TRACE("Loading texture: {}", filePath);

  if (!textureData.loadFromFile(filePath))
  {
    VRM_LOG_ERROR("Failed to load texture: {}", filePath);
    return false;
  }

  if (!_compareTextureWithNegX(textureData))
  {
    return false;
  }

  return true;
}

bool CubemapAsset::_compareTextureWithNegX(const ByteTextureData& tex) const
{
  VRM_CHECK_RET_FALSE_MSG(m_data.posx.getWidth() == tex.getWidth() && m_data.posx.getHeight() == tex.getHeight(), "Texture sizes don't match");
  VRM_CHECK_RET_FALSE_MSG(m_data.posx.getFormat() == tex.getFormat(), "Texture formats don't match");

  return true;
}
