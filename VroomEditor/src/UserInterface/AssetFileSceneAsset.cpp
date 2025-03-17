#include "VroomEditor/UserInterface/AssetFileSceneAsset.h"

using namespace vrm;

AssetFileSceneAsset::AssetFileSceneAsset(const std::filesystem::path &path)
    : AssetFile(path)
{
}

AssetFileSceneAsset::~AssetFileSceneAsset()
{
}

std::string AssetFileSceneAsset::getPictoAssetName() const
{
  return AssetFile::getPictoAssetName();
}
