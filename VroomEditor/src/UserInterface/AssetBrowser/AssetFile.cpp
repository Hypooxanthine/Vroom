#include "VroomEditor/UserInterface/AssetBrowser/AssetFile.h"

#include <imgui.h>

using namespace vrm;

AssetFile::AssetFile(const std::filesystem::path& path)
  : AssetElement(path)
{

}

AssetFile::~AssetFile()
{

}

std::string AssetFile::getPictoAssetName() const
{
  return "Resources/Editor/Picto/file.png";
}
