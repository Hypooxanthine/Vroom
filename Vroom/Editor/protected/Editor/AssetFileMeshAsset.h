#pragma once

#include "Editor/AssetFile.h"

namespace vrm
{
  
class AssetFileMeshAsset : public AssetFile
{
public:
  AssetFileMeshAsset(const std::filesystem::path& path);
  virtual ~AssetFileMeshAsset();

protected:
  virtual std::string getPictoAssetName() const override;
  virtual void onAddCustomBehaviour() override;

private:
};

} // namespace vrm
