#pragma once

#include "AssetManager/Api.h"
#include "AssetManager/AssetHandle.h"
#include "AssetManager/StaticAsset.h"

namespace vrm
{

class VRM_ASSETMANAGER_API ScriptAsset : public StaticAsset
{
public:

  using Handle = AssetHandle<ScriptAsset>;

public:

  ScriptAsset();
  ~ScriptAsset();

  inline const std::string& getScriptId() const
  {
    return getFilePath();
  }

protected:

  bool loadImpl(const std::string& filePath) override;
};

} // namespace vrm
