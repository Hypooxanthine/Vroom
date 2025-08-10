#include "VroomEditor/UserInterface/AssetBrowser/AssetFileMeshAsset.h"

using namespace vrm;


AssetFileMeshAsset::AssetFileMeshAsset(const std::filesystem::path& path)
  : AssetFile(path)
{
  
}

AssetFileMeshAsset::~AssetFileMeshAsset()
{

}

std::string AssetFileMeshAsset::getPictoAssetName() const
{
  return "Resources/Editor/Picto/mesh_asset.png";
}

void AssetFileMeshAsset::onAddCustomBehaviour()
{
  if (ImGui::BeginDragDropSource())
  {
    std::string pathStr = getPath().filename().string();
    ImGui::SetDragDropPayload("MeshAsset", &getPath(), sizeof(getPath()));
    ImGui::Text("%s", pathStr.c_str());
    ImGui::EndDragDropSource();
  }
}
