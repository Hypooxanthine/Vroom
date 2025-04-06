#include "VroomEditor/UserInterface/AssetBrowser/AssetFileMeshAsset.h"

#include "Vroom/Core/Log.h"

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
  return AssetFile::getPictoAssetName();
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
