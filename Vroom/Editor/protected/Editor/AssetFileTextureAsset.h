#pragma once

#include "Editor/AssetFile.h"

namespace vrm
{
  
class AssetFileTextureAsset : public AssetFile
{
public:
  AssetFileTextureAsset(const std::filesystem::path& path);
  virtual ~AssetFileTextureAsset();

protected:
  std::string getPictoAssetName() const override;
  void onAddCustomImGuiBehaviour() override;

private:
};

} // namespace vrm
