#include "VroomEditor/UserInterface/AssetBrowser/AssetDirectory.h"

#include <imgui.h>

#include "VroomEditor/EditorLayer.h"
#include "VroomEditor/UserInterface/AssetBrowser/AssetBrowser.h"

using namespace vrm;

AssetDirectory::AssetDirectory(const std::filesystem::path& path)
  : AssetElement(path)
{

}

AssetDirectory::~AssetDirectory()
{

}

std::string AssetDirectory::getPictoAssetName() const
{
  return "Resources/Editor/Picto/directory.png";
}

void AssetDirectory::onDoubleClick()
{
  EditorLayer::Get().pushRoutine([this](auto& layer){
    getBrowser().setCurrentPath(getPath());
  });
}
