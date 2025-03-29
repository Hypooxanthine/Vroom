#pragma once

#include "VroomEditor/UserInterface/ImGuiElement.h"
#include "imgui.h"

namespace vrm
{

  namespace gl
  {
    class FrameBuffer;
  }

  class Viewport : public ImGuiElement
  {
  public:
    Viewport();
    ~Viewport();

    inline const ImVec2 &getLastViewportSize() const { return m_LastViewportSize; }
    inline bool didSizeChangeLastFrame() const { return m_DidSizeChangeLastFrame; }

    inline gl::FrameBuffer *getFrameBuffer() const { return frameBuffer; }

    inline void setIdle()
    {
      m_Playing = false;
      m_Simulating = false;
    }

    inline bool isPlaying() const { return m_Playing; }
    inline bool isSimulating() const { return m_Simulating; }
    inline bool isPaused() const { return m_Paused; }
    inline bool isIdle() const { return !m_Playing && !m_Simulating; }
    inline bool isActive() const { return m_Active; }

    inline void setFrameBuffer(gl::FrameBuffer *fb) { frameBuffer = fb; }

  protected:
    bool onImgui() override;

  private: // ImGui related variables
    gl::FrameBuffer *frameBuffer;

    bool m_Active = false;
    bool m_Playing = false;
    bool m_Simulating = false;
    bool m_Paused = false;

  private:
    bool m_DidSizeChangeLastFrame = false;
    ImVec2 m_LastViewportSize;
  };

} // namespace vrm
