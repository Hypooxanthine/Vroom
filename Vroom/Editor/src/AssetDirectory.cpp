#include "Editor/AssetDirectory.h"

#include <imgui.h>

#include "Editor/EditorLayer.h"
#include "Editor/AssetBrowser.h"

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
  EditorLayer::Get().pushFrameEndRoutine([this](auto& layer){
    getBrowser().setCurrentPath(getPath());
  });
}
