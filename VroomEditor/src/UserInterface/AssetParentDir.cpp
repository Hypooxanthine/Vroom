#include "VroomEditor/UserInterface/AssetParentDir.h"

#include "Vroom/Asset/StaticAsset/TextureAsset.h"

#include <imgui.h>

using namespace vrm;

AssetParentDir::AssetParentDir(const std::filesystem::path& path)
    : AssetDirectory(path)
{

}

AssetParentDir::~AssetParentDir()
{

}

void AssetParentDir::onDrawPicto()
{
  if (m_Picto.getStaticAsset() == nullptr)
    m_Picto = getPicto();

  ImVec2 imgSize = { 60.f, 60.f };
  ImGui::SetCursorPosX((100.f - imgSize.x) / 2.f);
  ImGui::SetCursorPosY((100.f - imgSize.y) / 2.f);
  ImGui::Image(
      (ImTextureID)(intptr_t)m_Picto.getStaticAsset()->getGPUTexture().getRendererID(),
      imgSize,
      ImVec2(0, 1), ImVec2(1, 0));
}

std::string AssetParentDir::getText() const
{
    return "";
}

std::string AssetParentDir::getPictoAssetName() const
{
  return "Resources/Editor/Picto/parent_dir.png";
}
