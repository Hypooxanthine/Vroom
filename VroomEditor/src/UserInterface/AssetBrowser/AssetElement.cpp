#include "VroomEditor/UserInterface/AssetBrowser/AssetElement.h"

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/StaticAsset/TextureAsset.h"

#include "VroomEditor/EditorLayer.h"
#include "VroomEditor/UserInterface/AssetBrowser/AssetBrowser.h"
#include "VroomEditor/UserInterface/AssetBrowser/AssetUtils.h"

#include <Vroom/Core/Log.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

using namespace vrm;

ImVec2 AssetElement::s_ElementSize = { 100.f, 100.f };

AssetElement::AssetElement(const std::filesystem::path& elementPath)
  : ImGuiElement(), m_ElementPath((elementPath))
{
}

AssetElement::~AssetElement()
{
}

bool AssetElement::onImgui()
{
  bool ret = false;

  if (m_selected)
  {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyle().Colors[ImGuiCol_FrameBg]);
  }

  ImGui::PushID(ImGui::GetID(getPath().c_str()));

  auto childFlags = ImGuiChildFlags_Borders
    ;
  constexpr auto windowFlags = ImGuiWindowFlags_None | ImGuiWindowFlags_ChildWindow;

  auto cursor = ImGui::GetCursorPos();
  if (ImGui::BeginChild("##element", GetElementSize(), childFlags, windowFlags))
  {
    ImGui::SetWindowFontScale(0.8f);
    onDrawPicto();
    if (m_renaming)
    {
      onRenaming();
    }
    else
    {
      onDrawText();
    }
  }
  ImGui::EndChild();

  if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
    onClick();
  if (m_rightClickable && ImGui::BeginPopupContextItem("popup", ImGuiPopupFlags_MouseButtonRight))
  {
    contextualBehaviour();

    ImGui::EndPopup();
  }

  ImGui::SetCursorPos(cursor);
  static constexpr auto flags = ImGuiButtonFlags_MouseButtonLeft | (ImGuiButtonFlags_AllowOverlap | ImGuiButtonFlags_FlattenChildren);
  ImGui::InvisibleButton("##interact", ImGui::GetItemRectSize(), flags);

  if (ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()))
  {
    if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
    {
      onDoubleClick();
    }
  }
  else
  {
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
      requestUnselectThis();
    }
  }

  onAddCustomBehaviour();

  ImGui::PopID();

  if (m_selected)
  {
    ImGui::PopStyleColor();

    if (ImGui::IsKeyPressed(ImGuiKey_Delete))
    {
      requestDeleteThis();
    }

    if (ImGui::IsKeyPressed(ImGuiKey_Escape))
    {
      requestUnselectThis();
    }

    if (ImGui::IsKeyPressed(ImGuiKey_F2))
    {
      setRenaming();
    }
  }

  return ret;
}


void AssetElement::onDrawPicto()
{
  if (!m_Picto.isValid())
    m_Picto = getPicto();

  ImVec2 imgSize = { GetElementSize().x * 0.6f, GetElementSize().y * 0.6f };
  ImGui::SetCursorPosX((GetElementSize().x - imgSize.x) / 2.f);
  ImGui::Image(
    (ImTextureID)(intptr_t)m_Picto->getGPUTexture().getRenderId(),
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

void AssetElement::onRenaming()
{
  std::string fileName = getText();

  constexpr auto flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank;

  if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
    ImGui::SetKeyboardFocusHere(0);

  if (ImGui::InputText("##FileNameEdit", &fileName, flags))
  {
    if (fileName.empty())
    {
      if (auto* state = ImGui::GetInputTextState(ImGui::GetItemID()))
      {
        state->ReloadUserBufAndKeepSelection();
      }
    }
    else
    {
      std::filesystem::path newPath = getPath();
      newPath.remove_filename();
      newPath = newPath / fileName;

      EditorLayer::Get().pushRoutine([this, newPath](auto& layer) {
        AssetUtils::RenameAssetElement(getPath(), newPath);
        getBrowser().updateDirectoryContent();
      });
    }
    
    m_renaming = false;
  }


  if (ImGui::IsKeyPressed(ImGuiKey_Escape) || !ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows))
  {
    m_renaming = false;
  }
}

std::string AssetElement::getText() const
{
  return getPath().filename().string();
}

void AssetElement::onClick()
{
  if (!m_isSelectable)
    return;

  EditorLayer::Get().pushRoutine([this](auto& layer) {
    getBrowser().toggleSelectElement(this);
    });
}

void AssetElement::contextualBehaviour()
{
  if (!m_isTextEditable) ImGui::BeginDisabled();
  {
    if (ImGui::Selectable("Rename"))
    {
      setRenaming();
    }

    if (!m_isTextEditable) ImGui::EndDisabled();
  }

  if (!m_deletable) ImGui::BeginDisabled();
  {
    if (ImGui::Selectable("Delete"))
    {
      requestDeleteThis();
    }

    if (!m_deletable) ImGui::EndDisabled();
  }
}

void AssetElement::requestDeleteThis()
{
  EditorLayer::Get().pushRoutine([this](auto& layer) {
    AssetUtils::DeleteAssetElement(getPath());
    getBrowser().updateDirectoryContent();
    });
}

void AssetElement::requestUnselectThis()
{
  if (m_selected)
  {
    EditorLayer::Get().pushRoutine([this](auto& layer) {
      getBrowser().unselectElement();
      });
  }
}
