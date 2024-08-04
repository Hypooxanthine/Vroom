#pragma once

#include "VroomEditor/UserInterface/ImGuiElement.h"
#include "imgui.h"

namespace vrm
{

class FrameBuffer;

class Viewport : public ImGuiElement
{
public:
    Viewport();
    ~Viewport();

    inline const ImVec2& getLastViewportSize() const { return m_LastViewportSize; }
    inline bool didSizeChangeLastFrame() const { return m_DidSizeChangeLastFrame; }

protected:
    void onImgui() override;

public: // Public ImGui related variables
    FrameBuffer* frameBuffer;

private:
    bool m_DidSizeChangeLastFrame;
    ImVec2 m_LastViewportSize;

};

} // namespace vrm
