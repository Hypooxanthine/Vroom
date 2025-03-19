#pragma once

#include "VroomEditor/UserInterface/AssetBrowser/AssetElement.h"

namespace vrm
{
  
class AssetFile : public AssetElement
{
public:
  AssetFile(const std::filesystem::path& path);
  virtual ~AssetFile();

protected:
  virtual std::string getPictoAssetName() const override;

private:
};

} // namespace vrm
