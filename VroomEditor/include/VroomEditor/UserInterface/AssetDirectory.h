#pragma once

#include "VroomEditor/UserInterface/AssetElement.h"

namespace vrm
{

class AssetDirectory : public AssetElement
{
public:
  AssetDirectory(const std::filesystem::path &path);
  virtual ~AssetDirectory();

protected:
  virtual std::string getPictoAssetName() const override;

private:
};

} // namespace vrm
