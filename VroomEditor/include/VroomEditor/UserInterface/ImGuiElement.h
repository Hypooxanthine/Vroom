#pragma once

#include <string_view>

namespace vrm
{

class ImGuiElement
{
public:
    ImGuiElement();
    ~ImGuiElement();

    bool renderImgui();

protected:
    virtual bool onImgui() = 0;
    void notImplemented();

};

} // namespace vrm