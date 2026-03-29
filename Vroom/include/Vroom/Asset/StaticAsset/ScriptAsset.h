#pragma once

#include "Vroom/Api.h"
#include "Vroom/Asset/AssetHandle.h"
#include "Vroom/Asset/StaticAsset/StaticAsset.h"

namespace vrm
{

class VRM_API ScriptAsset : public StaticAsset
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
