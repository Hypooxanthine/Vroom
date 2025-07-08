#include "VroomEditor/UserInterface/AssetBrowser/AssetFileSceneAsset.h"

#include "VroomEditor/EditorLayer.h"

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
  return "Resources/Editor/Picto/scene_asset.png";
}

void AssetFileSceneAsset::onDoubleClick()
{
  EditorLayer::Get().loadScene(getPath().string());
}
