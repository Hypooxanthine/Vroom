#pragma once

#include "VroomEditor/UserInterface/AssetBrowser/AssetFile.h"

namespace vrm
{
  
class AssetFileMaterialAsset : public AssetFile
{
public:
  AssetFileMaterialAsset(const std::filesystem::path& path);
  virtual ~AssetFileMaterialAsset();

protected:
  virtual std::string getPictoAssetName() const override;
  virtual void onAddCustomBehaviour() override;

private:
};

} // namespace vrm
