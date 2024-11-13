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

    inline FrameBuffer* getFrameBuffer() const { return frameBuffer; }

    inline bool requestedPlay() const { return m_PressedPlay; }
    inline bool requestedPause() const { return m_PressedPause; }
    inline bool requestedStop() const { return m_PressedStop; }
    inline bool isActive() const { return m_Active; }

    inline void setFrameBuffer(FrameBuffer* fb) { frameBuffer = fb; }

protected:
    void onImgui() override;

private: // ImGui related variables
    FrameBuffer* frameBuffer;

    bool m_PressedPlay = false;
    bool m_PressedPause = false;
    bool m_PressedStop = false;
    bool m_Active = false;

private:
    bool m_DidSizeChangeLastFrame = false;
    ImVec2 m_LastViewportSize;

};

} // namespace vrm
