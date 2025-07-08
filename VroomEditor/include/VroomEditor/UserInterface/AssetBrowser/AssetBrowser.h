#pragma once

#include "VroomEditor/UserInterface/ImGuiElement.h"
#include "VroomEditor/UserInterface/AssetBrowser/AssetElement.h"

#include <filesystem>
#include <vector>

namespace vrm
{

  class AssetElement;
  class AssetFile;
  struct OSFileDrop;

  class AssetBrowser : public ImGuiElement
  {
  public:
    AssetBrowser(const std::filesystem::path &resourcesPath = "Resources/");
    ~AssetBrowser();

    void setCurrentPath(const std::filesystem::path &newPath);
    void updateDirectoryContent();

    void selectElement(const AssetElement* element);

  protected:
    bool onImgui() override;

  private:
    
    void _handleFileDrop(const OSFileDrop& dropData);

  private:
    const std::filesystem::path m_ResourcesPath;
    std::filesystem::path m_CurrentPath;

    std::vector<std::unique_ptr<AssetElement>> m_Assets;
    AssetElement* m_selectedAsset = nullptr;
  };

} // namespace vrm
