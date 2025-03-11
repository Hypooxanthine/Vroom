#pragma once

#include "VroomEditor/UserInterface/AssetElement.h"

namespace vrm
{

class AssetDirectory : public AssetElement
{
public:
    AssetDirectory(const std::filesystem::path& path);
    virtual ~AssetDirectory();

protected:
    bool onImgui() override;
    virtual std::string getDisplayText() const;

private:

};

} // namespace vrm
