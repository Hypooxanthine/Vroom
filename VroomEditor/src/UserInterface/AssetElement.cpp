#include "VroomEditor/UserInterface/AssetElement.h"

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/StaticAsset/TextureAsset.h"

#include <Vroom/Core/Log.h>
#include <imgui.h>

using namespace vrm;

AssetElement::AssetElement(const std::filesystem::path &elementPath)
    : ImGuiElement(), m_ElementPath(std::filesystem::canonical(elementPath))
{
}

AssetElement::~AssetElement()
{
}

bool AssetElement::onImgui()
{
  bool ret = false;

  ImGui::PushID(ImGui::GetID(getPath().c_str()));

  constexpr auto childFlags =
      ImGuiChildFlags_Borders;
  constexpr auto windowFlags =
      ImGuiWindowFlags_None | ImGuiWindowFlags_ChildWindow;

  if (ImGui::BeginChild("##element", {100.f, 100.f}, childFlags, windowFlags))
  {
    ImGui::SetWindowFontScale(0.8f);
    onDrawPicto();
    onDrawText();
  }
  ImGui::EndChild();

  ImGui::PopID();

  return ret;
}

void AssetElement::onDrawPicto()
{
  if (m_Picto.getStaticAsset() == nullptr)
    m_Picto = getPicto();

  ImVec2 imgSize = { 60.f, 60.f };
  ImGui::SetCursorPosX((100.f - imgSize.x) / 2.f);
  ImGui::Image(
      (ImTextureID)(intptr_t)m_Picto.getStaticAsset()->getGPUTexture().getRendererID(),
      imgSize,
      ImVec2(0, 1), ImVec2(1, 0));
}

TextureInstance AssetElement::getPicto() const
{
  const auto picto = AssetManager::Get().getAsset<TextureAsset>(getPictoAssetName());
  return picto;
}

std::string AssetElement::getPictoAssetName() const
{
  return std::string(GetDefaultPictoAssetName().data());
}

void AssetElement::onDrawText()
{
  auto txt = getText();
  if (txt.empty())
    return;
  
  auto windowWidth = ImGui::GetWindowSize().x;
  auto textSize = ImGui::CalcTextSize(txt.c_str()).x;

  bool textTooLong = textSize > windowWidth;

  if (!textTooLong)
    ImGui::SetCursorPosX((windowWidth - textSize) * 0.5f);

  ImGui::Text("%s", txt.c_str());

  if (textTooLong)
  {
    ImGui::SetItemTooltip("%s", txt.c_str());
  }
}

std::string AssetElement::getText() const
{
  return getPath().filename().string();
}
