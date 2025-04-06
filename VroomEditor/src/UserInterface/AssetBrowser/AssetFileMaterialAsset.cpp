#include "VroomEditor/UserInterface/AssetBrowser/AssetFileMaterialAsset.h"

#include "Vroom/Core/Log.h"

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
  return AssetFile::getPictoAssetName();
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
