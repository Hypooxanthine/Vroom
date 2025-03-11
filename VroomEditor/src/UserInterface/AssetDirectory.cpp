#include "VroomEditor/UserInterface/AssetDirectory.h"

#include <imgui.h>

using namespace vrm;

AssetDirectory::AssetDirectory(const std::filesystem::path& path)
    : AssetElement(std::filesystem::canonical(path))
{

}

AssetDirectory::~AssetDirectory()
{

}

bool AssetDirectory::onImgui()
{
    bool ret = false;
    ImGui::PushID(getPath().c_str());

    if (ImGui::Button(getDisplayText().c_str()))
    {
        ret = true;
    }

    ImGui::PopID();

    return ret;
}

std::string AssetDirectory::getDisplayText() const
{
    return getPath().filename().string();
}
