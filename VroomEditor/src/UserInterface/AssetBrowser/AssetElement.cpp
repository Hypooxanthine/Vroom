#include "VroomEditor/UserInterface/AssetBrowser/AssetElement.h"

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/StaticAsset/TextureAsset.h"

#include <Vroom/Core/Log.h>
#include <imgui.h>
#include <imgui_internal.h>

using namespace vrm;

ImVec2 AssetElement::s_ElementSize = { 100.f, 100.f };

AssetElement::AssetElement(const std::filesystem::path &elementPath)
    : ImGuiElement(), m_ElementPath((elementPath))
{
}

AssetElement::~AssetElement()
{
}

bool AssetElement::onImgui()
{
  bool ret = false;
  m_Action = EAction::eNone;

  ImGui::PushID(ImGui::GetID(getPath().c_str()));

  constexpr auto childFlags = ImGuiChildFlags_Borders;
  constexpr auto windowFlags = ImGuiWindowFlags_None | ImGuiWindowFlags_ChildWindow;

  auto cursor = ImGui::GetCursorPos();
  if (ImGui::BeginChild("##element", GetElementSize(), childFlags, windowFlags))
  {
    ImGui::SetWindowFontScale(0.8f);
    onDrawPicto();
    onDrawText();
  }
  ImGui::EndChild();

  ImGui::SetCursorPos(cursor);
  static constexpr auto flags = ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_AllowOverlap | ImGuiButtonFlags_FlattenChildren;
  ImGui::InvisibleButton("##interact", ImGui::GetItemRectSize(), flags);

  if (ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()) && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
  {
    auto action = onDoubleClick();
    if (action != EAction::eNone)
    {
      m_Action = action;
      ret = true;
    }
  }

  onAddCustomBehaviour();

  ImGui::PopID();
  return ret;
}


void AssetElement::onDrawPicto()
{
  if (!m_Picto.isValid())
    m_Picto = getPicto();

  ImVec2 imgSize = {GetElementSize().x * 0.6f, GetElementSize().y * 0.6f};
  ImGui::SetCursorPosX((GetElementSize().x - imgSize.x) / 2.f);
  ImGui::Image(
      (ImTextureID)(intptr_t)m_Picto->getGPUTexture().getRendererID(),
      imgSize,
      ImVec2(0, 1), ImVec2(1, 0));
}

TextureAsset::Handle AssetElement::getPicto() const
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
