#include "Editor/AssetFileMaterialAsset.h"
#include "AssetManager/AssetManager.h"
#include "AssetManager/MaterialAsset.h"
#include "Core/Log.h"
#include "Editor/UserInterfaceLayer.h"
#include "Editor/MaterialEditor.h"

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

void AssetFileMaterialAsset::onAddCustomImGuiBehaviour()
{
  if (ImGui::BeginDragDropSource())
  {
    std::string pathStr = getPath().filename().string();
    ImGui::SetDragDropPayload("MaterialAsset", &getPath(), sizeof(getPath()));
    ImGui::Text("%s", pathStr.c_str());
    ImGui::EndDragDropSource();
  }
}

void AssetFileMaterialAsset::onDoubleClick()
{
  MaterialAsset::Handle material = AssetManager::Get().tryGetAsset<MaterialAsset>(getPath());
  
  if (material.isValid())
    VRM_EDITOR_UI_ELEMENT(MaterialEditor).open(material);
  else
    VRM_LOG_ERROR("Could not find material {}", getPath().string());
}
