#pragma once

#include "imgui.h"
#include "imgui_internal.h"

#include "VroomEditor/UserInterface/ImGuiElement.h"

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/StaticAsset/CubemapAsset.h"
#include "Vroom/Asset/StaticAsset/MaterialAsset.h"
#include "Vroom/Asset/StaticAsset/MeshAsset.h"
#include "misc/cpp/imgui_stdlib.h"

namespace vrm
{
template <typename AssetType>
class AssetSelector : public ImGuiElement
{
public:

  inline AssetSelector() {}
  inline AssetSelector(const AssetType::Handle& asset) : m_asset(asset) {}

  inline ~AssetSelector() {}

  AssetSelector& operator=(const AssetSelector& other) = delete;
  AssetSelector(const AssetSelector& other)            = delete;

  AssetSelector& operator=(AssetSelector&& other) = default;
  AssetSelector(AssetSelector&& other)            = default;

  inline void setAsset(const AssetType::Handle& asset) { m_asset = asset; }

  inline AssetType::Handle getAsset() const { return m_asset; }
  inline bool              getChanged() const { return m_justChanged; }

protected:

  void onImgui() override;

private:

  AssetType::Handle m_asset       = {};
  bool              m_justChanged = false;
};

using MeshSelector     = AssetSelector<MeshAsset>;
using MaterialSelector = AssetSelector<MaterialAsset>;
using CubemapSelector  = AssetSelector<CubemapAsset>;

template <typename SelectorType>
struct AssetSelectorTraits;

template <>
struct AssetSelectorTraits<MeshAsset>
{
  static consteval const char* GetDisplayName() { return "Mesh"; }
  static consteval const char* GetDragDropTargetName() { return "MeshAsset"; }
};

template <>
struct AssetSelectorTraits<MaterialAsset>
{
  static consteval const char* GetDisplayName() { return "Material"; }
  static consteval const char* GetDragDropTargetName()
  {
    return "MaterialAsset";
  }
};

template <>
struct AssetSelectorTraits<CubemapAsset>
{
  static consteval const char* GetDisplayName() { return "Cubemap"; }
  static consteval const char* GetDragDropTargetName()
  {
    return "CubemapAsset";
  }
};

template <typename AssetType>
void AssetSelector<AssetType>::onImgui()
{
  m_justChanged            = false;
  bool        assetChanged = false;
  std::string assetName    = m_asset.isValid() ? m_asset->getFilePath() : "";

  constexpr auto flags = ImGuiInputTextFlags_AutoSelectAll
                       | ImGuiInputTextFlags_EnterReturnsTrue
                       | ImGuiInputTextFlags_CharsNoBlank;

  if (ImGui::InputText(AssetSelectorTraits<AssetType>::GetDisplayName(),
                       &assetName, flags))
  {
    if (AssetManager::Get().tryLoadAsset<AssetType>(assetName))
    {
      assetChanged = true;
    }
    else
    {
      if (auto* state = ImGui::GetInputTextState(ImGui::GetItemID()))
      {
        state->ReloadUserBufAndKeepSelection();
      }
    }
  }

  float itemWidth = ImGui::GetItemRectMin().x - ImGui::GetItemRectMax().x;
  float textWidth = ImGui::CalcTextSize(assetName.c_str()).x;

  if (textWidth > itemWidth)
  {
    ImGui::SetItemTooltip(assetName.c_str());
  }

  if (ImGui::BeginDragDropTarget())
  {
    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(
          AssetSelectorTraits<AssetType>::GetDragDropTargetName()))
    {
      // Entity* newChild = (Entity*)payload->Data;
      std::filesystem::path meshPath = *(std::filesystem::path*)payload->Data;

      if (AssetManager::Get().tryLoadAsset<AssetType>(meshPath.string()))
      {
        assetName    = meshPath.string();
        assetChanged = true;
      }
    }

    ImGui::EndDragDropTarget();
  }

  if (assetChanged)
  {
    typename AssetType::Handle requestNewMesh =
      AssetManager::Get().tryGetAsset<AssetType>(assetName);

    if (requestNewMesh.isValid() && requestNewMesh != m_asset)
    {
      m_asset       = requestNewMesh;
      m_justChanged = true;
    }
  }
}

} // namespace vrm
