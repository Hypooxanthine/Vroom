#pragma once

#include "VroomEditor/UserInterface/AssetBrowser/AssetElement.h"

namespace vrm
{

class AssetDirectory : public AssetElement
{
public:
  AssetDirectory(const std::filesystem::path &path);
  virtual ~AssetDirectory();

protected:
  virtual std::string getPictoAssetName() const override;
  virtual void onDoubleClick() override;

private:
};

} // namespace vrm
