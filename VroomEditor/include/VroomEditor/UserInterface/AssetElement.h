#pragma once

#include "Vroom/Asset/AssetInstance/TextureInstance.h"
#include "VroomEditor/UserInterface/ImGuiElement.h"

#include <filesystem>
#include <imgui.h>

namespace vrm
{

class AssetElement : public ImGuiElement
{
public:
  AssetElement(const std::filesystem::path &elementPath);
  virtual ~AssetElement();

  inline const std::filesystem::path &getPath() const { return m_ElementPath; }
  inline void setPath(const std::filesystem::path &path) { m_ElementPath = std::filesystem::canonical(path); }

  inline static const auto& GetElementSize() { return s_ElementSize; }
  inline static void SetElementSize(const ImVec2& size) { s_ElementSize = size; }

protected:
  virtual bool onImgui() override;
  virtual void onDrawPicto();
  virtual TextureInstance getPicto() const;
  inline static consteval std::string_view GetDefaultPictoAssetName() { return "Resources/Editor/Picto/unknown.png"; }
  virtual std::string getPictoAssetName() const;
  virtual void onDrawText();
  virtual std::string getText() const;

protected:
  static ImVec2 s_ElementSize;
  TextureInstance m_Picto;

private:
  std::filesystem::path m_ElementPath;
};

} // namespace vrm
