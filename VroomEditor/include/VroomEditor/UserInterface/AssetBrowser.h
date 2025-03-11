#pragma once

#include "VroomEditor/UserInterface/ImGuiElement.h"

#include <filesystem>
#include <vector>

namespace vrm
{

class AssetElement;

class AssetBrowser : public ImGuiElement
{
public:
    AssetBrowser(const std::filesystem::path& resourcesPath = "Resources/");
    ~AssetBrowser();

    void setCurrentPath(const std::filesystem::path& newPath);
    void updateDirectoryContent();

protected:
    bool onImgui() override;

private:
    const std::filesystem::path m_ResourcesPath;
    std::filesystem::path m_CurrentPath;

    std::vector<std::unique_ptr<AssetElement>> m_Assets;
};

} // namespace vrm
