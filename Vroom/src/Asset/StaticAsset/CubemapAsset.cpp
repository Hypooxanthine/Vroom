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
  VRM_LOG_INFO("Loading skybox: {}", filePath);
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

  VRM_CHECK_RET_FALSE_MSG(_buildGpuCubemap(), "Could not create GPU cubemap");

  VRM_LOG_INFO("Skybox loaded: {}", filePath);
  
  return true;
}

bool CubemapAsset::_loadTexture(const std::string& filePath, ByteTextureData& textureData)
{
  VRM_LOG_TRACE("Loading texture: {}", filePath);

  if (!textureData.loadFromFile(applyPathOrder(filePath), false))
  {
    VRM_LOG_ERROR("Failed to load texture: {}", filePath);
    return false;
  }

  return _compareTextureWithNegX(textureData);
}

bool CubemapAsset::_compareTextureWithNegX(const ByteTextureData& tex) const
{
  VRM_CHECK_RET_FALSE_MSG(m_data.posx.getWidth() == tex.getWidth() && m_data.posx.getHeight() == tex.getHeight(), "Texture sizes don't match");
  VRM_CHECK_RET_FALSE_MSG(m_data.posx.getFormat() == tex.getFormat(), "Texture formats don't match");

  return true;
}

bool CubemapAsset::_buildGpuCubemap()
{
  gl::Texture::Desc desc;
  {
    desc.width = m_data.posx.getWidth();
    desc.height = m_data.posx.getHeight();
    desc.dimension = 2;
    desc.cubemap = true;
    desc.mipmapCount = gl::Texture::GetMaxMipMapCount(desc.width, desc.height);

    switch (m_data.posx.getChannels())
    {
    case 1:
      desc.internalFormat = GL_R8;
      desc.format = GL_R;
      break;
    case 2:
      desc.internalFormat = GL_RG8;
      desc.format = GL_RG;
      break;
    case 3:
      desc.internalFormat = GL_RGB8;
      desc.format = GL_RGB;
      break;
    case 4:
      desc.internalFormat = GL_RGBA8;
      desc.format = GL_RGBA;
      break;
    default:
      VRM_CHECK_RET_FALSE_MSG(false, "Unexpected texture channel count");
      break;
    }
  }

  GLCall(m_gpuTexture.create(desc));

  glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0, 0, desc.width, desc.height, desc.format, GL_UNSIGNED_BYTE, m_data.posx.getData());
  glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, 0, 0, desc.width, desc.height, desc.format, GL_UNSIGNED_BYTE, m_data.negx.getData());
  glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, 0, 0, desc.width, desc.height, desc.format, GL_UNSIGNED_BYTE, m_data.posy.getData());
  glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, 0, 0, desc.width, desc.height, desc.format, GL_UNSIGNED_BYTE, m_data.negy.getData());
  glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, 0, 0, desc.width, desc.height, desc.format, GL_UNSIGNED_BYTE, m_data.posz.getData());
  glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, 0, 0, desc.width, desc.height, desc.format, GL_UNSIGNED_BYTE, m_data.negz.getData());
  glGenerateMipmap(GL_TEXTURE_2D);
  
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  return true;
}
