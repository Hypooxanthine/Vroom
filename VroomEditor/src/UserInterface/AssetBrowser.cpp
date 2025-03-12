#include "VroomEditor/UserInterface/AssetBrowser.h"

#include <imgui.h>

#include "VroomEditor/UserInterface/AssetDirectory.h"
#include "VroomEditor/UserInterface/AssetParentDir.h"

#include "Vroom/Core/Log.h"

using namespace vrm;

AssetBrowser::AssetBrowser(const std::filesystem::path& resourcesPath)
    : ImGuiElement(),
    m_ResourcesPath(std::filesystem::canonical(resourcesPath)),
    m_CurrentPath(m_ResourcesPath)
{
    updateDirectoryContent();
}

AssetBrowser::~AssetBrowser()
{

}

static bool IsChildOf(const std::filesystem::path& parent, const std::filesystem::path& child)
{
    std::filesystem::path relative = child.lexically_relative(parent);
    return !relative.empty() && relative.native()[0] != '.';
}

void AssetBrowser::setCurrentPath(const std::filesystem::path& newPath)
{
    auto p = std::filesystem::canonical(newPath);

    if (p != m_CurrentPath
        && (
            p == m_ResourcesPath
            || IsChildOf(m_ResourcesPath, p)
        ))
    {
        m_CurrentPath = newPath;
        updateDirectoryContent();
    }
}

void AssetBrowser::updateDirectoryContent()
{
    m_Assets.clear();

    if (m_CurrentPath != m_ResourcesPath)
    {
        m_Assets.emplace_back().reset(new AssetParentDir(m_CurrentPath.parent_path()));
    }

    for (const auto& entry : std::filesystem::directory_iterator(m_CurrentPath))
    {
        if (entry.is_directory())
        {
            m_Assets.emplace_back().reset(new AssetDirectory(entry.path()));
        }
        else if (entry.is_regular_file())
        {

        }
    }
}

bool AssetBrowser::onImgui()
{
    bool ret = false;

    std::filesystem::path nextPath = m_CurrentPath;

    if (ImGui::Begin("Asset browser"))
    {
        ImGui::PushID("Path");
            ImGui::TextWrapped("%s/", m_CurrentPath.lexically_relative(m_ResourcesPath.parent_path()).c_str());
        ImGui::PopID();

        bool first = true;

        for (auto& elem : m_Assets)
        {
            if (first)
            {
                first = false;
            }
            else
            {
                ImGui::SameLine();
            }

            if (elem->renderImgui())
            {
                nextPath = elem->getPath();
                ret = true;
            }
        }
    }
    ImGui::End();

    setCurrentPath(nextPath);

    return ret;
}
