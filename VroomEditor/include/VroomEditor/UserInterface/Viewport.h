#pragma once

#include "VroomEditor/UserInterface/ImGuiElement.h"

namespace vrm
{

class FrameBuffer;

class Viewport : public ImGuiElement
{
public:
    Viewport();
    ~Viewport();

protected:
    void onImgui() override;

public: // Public ImGui related variables
    FrameBuffer* frameBuffer;

};

} // namespace vrm
