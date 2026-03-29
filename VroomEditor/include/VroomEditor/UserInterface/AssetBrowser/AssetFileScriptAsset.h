#pragma once

#include "VroomEditor/UserInterface/AssetBrowser/AssetFile.h"

namespace vrm
{

class AssetFileScriptAsset : public AssetFile
{
public:

  AssetFileScriptAsset(const std::filesystem::path& path);
  virtual ~AssetFileScriptAsset();

protected:

  virtual std::string getPictoAssetName() const override;
  virtual void onAddCustomBehaviour() override;
};

} // namespace vrm
