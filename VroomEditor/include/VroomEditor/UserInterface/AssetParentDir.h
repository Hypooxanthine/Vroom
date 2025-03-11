#pragma once

#include "VroomEditor/UserInterface/AssetDirectory.h"

namespace vrm
{

class AssetParentDir : public AssetDirectory
{
public:
    AssetParentDir(const std::filesystem::path& path);
    ~AssetParentDir();

protected:
    std::string getDisplayText() const override;

private:

};

} // namespace vrm
