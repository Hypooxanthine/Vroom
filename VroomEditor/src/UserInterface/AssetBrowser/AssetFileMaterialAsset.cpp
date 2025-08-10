#include "VroomEditor/UserInterface/AssetBrowser/AssetFileMaterialAsset.h"

using namespace vrm;


AssetFileMaterialAsset::AssetFileMaterialAsset(const std::filesystem::path& path)
  : AssetFile(path)
{
  
}

AssetFileMaterialAsset::~AssetFileMaterialAsset()
{

}

std::string AssetFileMaterialAsset::getPictoAssetName() const
{
  return "Resources/Editor/Picto/material_asset.png";
}

void AssetFileMaterialAsset::onAddCustomBehaviour()
{
  if (ImGui::BeginDragDropSource())
  {
    std::string pathStr = getPath().filename().string();
    ImGui::SetDragDropPayload("MaterialAsset", &getPath(), sizeof(getPath()));
    ImGui::Text("%s", pathStr.c_str());
    ImGui::EndDragDropSource();
  }
}
