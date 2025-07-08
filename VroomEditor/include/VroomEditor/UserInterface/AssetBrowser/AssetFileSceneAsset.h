#pragma once

#include "VroomEditor/UserInterface/AssetBrowser/AssetFile.h"

namespace vrm
{
  
class AssetFileSceneAsset : public AssetFile
{
public:
  AssetFileSceneAsset(const std::filesystem::path& path);
  virtual ~AssetFileSceneAsset();

protected:
  virtual std::string getPictoAssetName() const override;
  virtual void onDoubleClick() override;

private:
};

} // namespace vrm
