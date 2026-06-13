#include "Editor/AssetFileTextureAsset.h"
#include "AssetManager/AssetManager.h"
#include "AssetManager/MaterialAsset.h"
#include "AssetManager/TextureAsset.h"
#include "Core/Log.h"
#include "Editor/UserInterfaceLayer.h"
#include "Editor/MaterialEditor.h"

using namespace vrm;


AssetFileTextureAsset::AssetFileTextureAsset(const std::filesystem::path& path)
  : AssetFile(path)
{
  
}

AssetFileTextureAsset::~AssetFileTextureAsset()
{

}

std::string AssetFileTextureAsset::getPictoAssetName() const
{
  return "Resources/Editor/Picto/texture_asset.png";
}

void AssetFileTextureAsset::onAddCustomImGuiBehaviour()
{
  if (ImGui::BeginDragDropSource())
  {
    std::string pathStr = getPath().filename().string();
    ImGui::SetDragDropPayload("TextureAsset", &getPath(), sizeof(getPath()));
    ImGui::Text("%s", pathStr.c_str());
    ImGui::EndDragDropSource();
  }
}
