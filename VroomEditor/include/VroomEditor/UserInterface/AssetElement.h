#pragma once

#include "VroomEditor/UserInterface/ImGuiElement.h"

#include <filesystem>

namespace vrm
{

class AssetElement : public ImGuiElement
{
public:
    AssetElement(const std::filesystem::path& elementPath);
    virtual ~AssetElement();

    inline const std::filesystem::path& getPath() const { return m_ElementPath; }
    inline void setPath(const std::filesystem::path& path) { m_ElementPath = std::filesystem::canonical(path); }

protected:

private:
    std::filesystem::path m_ElementPath;
};

} // namespace vrm
