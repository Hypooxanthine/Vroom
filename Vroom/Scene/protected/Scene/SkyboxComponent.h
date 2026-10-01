#pragma once

#include "AssetManager/ComponentDataFwds.h"
#include "AssetManager/CubemapAsset.h"
#include "Scene/Api.h"

namespace vrm
{

class VRM_SCENE_API SkyboxComponent
{
public:

  SkyboxComponent();
  explicit SkyboxComponent(const SkyboxComponentData& data);
  SkyboxComponent(const CubemapAsset::Handle& cubemap);
  ~SkyboxComponent();

  SkyboxComponentData getData() const;

  CubemapAsset::Handle getCubemapAsset() const
  {
    return m_cubemap;
  }
  void setCubemapAsset(const CubemapAsset::Handle& cubemap)
  {
    m_cubemap = cubemap;
    m_dirtyForRender = true;
  }

  bool consumeDirtyForRender() const
  {
    bool value = m_dirtyForRender;
    m_dirtyForRender = false;

    return value;
  }

  void markDirtyForRender() const
  {
    m_dirtyForRender = true;
  }

private:

  CubemapAsset::Handle m_cubemap;
  mutable bool m_dirtyForRender = true;
};

} // namespace vrm
