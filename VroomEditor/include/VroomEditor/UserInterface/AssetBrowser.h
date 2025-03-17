#pragma once

#include "VroomEditor/UserInterface/ImGuiElement.h"
#include "VroomEditor/UserInterface/AssetElement.h"

#include <filesystem>
#include <vector>

namespace vrm
{

class AssetElement;
class AssetFile;

class AssetBrowser : public ImGuiElement
{
public:
    AssetBrowser(const std::filesystem::path& resourcesPath = "Resources/");
    ~AssetBrowser();

    void setCurrentPath(const std::filesystem::path& newPath);
    void updateDirectoryContent();

    inline AssetElement::EAction getAction() const { return m_Action; }
    inline const std::filesystem::path& getSelectedAsset() const { return m_SelectedAsset; }

protected:
    bool onImgui() override;

private:
    AssetFile* CreateAssetFile(const std::filesystem::path &path);

private:
    const std::filesystem::path m_ResourcesPath;
    std::filesystem::path m_CurrentPath;

    std::vector<std::unique_ptr<AssetElement>> m_Assets;
    AssetElement::EAction m_Action = AssetElement::EAction::eNone;
    std::filesystem::path m_SelectedAsset;
};

} // namespace vrm
