#pragma once

#include "Vroom/Asset/StaticAsset/TextureAsset.h"
#include "VroomEditor/UserInterface/ImGuiElement.h"

#include <filesystem>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>

namespace vrm
{

class AssetBrowser;

class AssetElement : public ImGuiElement
{
public:
  AssetElement(const std::filesystem::path &elementPath);
  virtual ~AssetElement();

  inline const std::filesystem::path &getPath() const { return m_ElementPath; }
  inline void setPath(const std::filesystem::path &path) { m_ElementPath = std::filesystem::canonical(path); }

  inline static const auto& GetElementSize() { return s_ElementSize; }
  inline static void SetElementSize(const ImVec2& size) { s_ElementSize = size; }

  inline void setBrowser(AssetBrowser* browser) { m_Browser = browser; }

  inline void setSelected(bool selected) { m_selected = selected; }
  inline void setRenaming() { m_renaming = true; }

protected:
  inline static consteval std::string_view GetDefaultPictoAssetName() { return "Resources/Editor/Picto/unknown.png"; }

  virtual void onImgui() override;
  virtual void onDrawPicto();
  virtual TextureAsset::Handle getPicto() const;
  virtual std::string getPictoAssetName() const;
  virtual std::string getText() const;
  virtual void onDoubleClick() {}
  virtual void onAddCustomBehaviour() {}

  inline AssetBrowser &getBrowser() { return *m_Browser; }
  inline const AssetBrowser &getBrowser() const { return *m_Browser; }

private:

  void onClick();
  void contextualBehaviour();
  void onDrawText();
  void onRenaming();

  void requestSelectThis();
  void requestDeleteThis();
  void requestUnselectThis();

protected:
  static ImVec2 s_ElementSize;
  TextureAsset::Handle m_Picto;
  bool m_isSelectable = true;
  bool m_rightClickable = true;
  bool m_isTextEditable = true;
  bool m_deletable = true;

private:
  std::filesystem::path m_ElementPath;
  AssetBrowser* m_Browser = nullptr;
  bool m_selected = false;
  bool m_renaming = false;
};

} // namespace vrm
