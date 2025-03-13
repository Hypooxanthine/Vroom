#pragma once

#include "VroomEditor/UserInterface/AssetDirectory.h"

namespace vrm
{

class AssetParentDir : public AssetDirectory
{
public:
  AssetParentDir(const std::filesystem::path& path);
  virtual ~AssetParentDir();

protected:
  virtual void onDrawPicto() override;
  virtual std::string getText() const override;
  virtual std::string getPictoAssetName() const override;

private:

};

} // namespace vrm
