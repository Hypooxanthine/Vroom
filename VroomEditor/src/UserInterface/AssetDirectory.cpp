#include "VroomEditor/UserInterface/AssetDirectory.h"

#include <imgui.h>

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
