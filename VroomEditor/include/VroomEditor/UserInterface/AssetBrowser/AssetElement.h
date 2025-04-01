#pragma once

#include "Vroom/Asset/StaticAsset/TextureAsset.h"
#include "VroomEditor/UserInterface/ImGuiElement.h"

#include <filesystem>
#include <imgui.h>

namespace vrm
{

class AssetBrowser;

class AssetElement : public ImGuiElement
{
public:
  enum class EAction
  {
    eNone = 0,
    eNavigate,
    eLoadScene
  };

public:
  AssetElement(const std::filesystem::path &elementPath);
  virtual ~AssetElement();

  inline const std::filesystem::path &getPath() const { return m_ElementPath; }
  inline void setPath(const std::filesystem::path &path) { m_ElementPath = std::filesystem::canonical(path); }

  inline static const auto& GetElementSize() { return s_ElementSize; }
  inline static void SetElementSize(const ImVec2& size) { s_ElementSize = size; }

  inline AssetBrowser &getBrowser() { return *m_Browser; }
  inline const AssetBrowser &getBrowser() const { return *m_Browser; }

  inline EAction getAction() const { return m_Action; }

protected:
  inline static consteval std::string_view GetDefaultPictoAssetName() { return "Resources/Editor/Picto/unknown.png"; }

  virtual bool onImgui() override;
  virtual void onDrawPicto();
  virtual TextureAsset::Handle getPicto() const;
  virtual std::string getPictoAssetName() const;
  virtual void onDrawText();
  virtual std::string getText() const;
  virtual EAction onDoubleClick() { return EAction::eNone; }

protected:
  static ImVec2 s_ElementSize;
  TextureAsset::Handle m_Picto;

private:
  std::filesystem::path m_ElementPath;
  AssetBrowser *m_Browser;
  EAction m_Action = EAction::eNone;
};

} // namespace vrm
