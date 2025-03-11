#include "VroomEditor/UserInterface/AssetParentDir.h"

using namespace vrm;

AssetParentDir::AssetParentDir(const std::filesystem::path& path)
    : AssetDirectory(path)
{

}

AssetParentDir::~AssetParentDir()
{

}

std::string AssetParentDir::getDisplayText() const
{
    return "<-";
}
