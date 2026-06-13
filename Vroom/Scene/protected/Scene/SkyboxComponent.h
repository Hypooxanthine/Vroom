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
  SkyboxComponent(CubemapAsset::Handle cubemap);
  ~SkyboxComponent();

  SkyboxComponentData getData() const;

  inline CubemapAsset::Handle getCubemapAsset() const
  {
    return m_cubemap;
  }
  inline void setCubemapAsset(CubemapAsset::Handle cubemap)
  {
    m_cubemap = cubemap;
    m_dirtyForRender = true;
  }

  inline bool consumeDirtyForRender() const
  {
    bool value       = m_dirtyForRender;
    m_dirtyForRender = false;

    return value;
  }

  inline void markDirtyForRender() const
  {
    m_dirtyForRender = true;
  }

private:

  CubemapAsset::Handle m_cubemap;
  mutable bool         m_dirtyForRender = true;
};

} // namespace vrm
