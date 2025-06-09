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
  virtual EAction onDoubleClick() override { return EAction::eNavigate; }

private:
};

} // namespace vrm
