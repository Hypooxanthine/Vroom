#pragma once

#include "VroomEditor/UserInterface/ImGuiElement.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"

namespace vrm
{

  namespace gl
  {
    class Texture2D;
  }

  class Viewport : public ImGuiElement
  {
  public:
    Viewport();
    ~Viewport();

    inline const ImVec2 &getLastViewportSize() const { return m_LastViewportSize; }
    inline bool didSizeChangeLastFrame() const { return m_DidSizeChangeLastFrame; }

    inline const gl::Texture2D *getRenderTexture() const { return renderTexture; }

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

    inline void setRenderTexture(const gl::Texture2D *tex) { renderTexture = tex; }

    inline void allowActivation() { m_clicking = false; } // Clicking state blocks viewport activation

  protected:
    bool onImgui() override;

  private: // ImGui related variables
    const gl::Texture2D *renderTexture;

    bool m_Active = false;
    bool m_Playing = false;
    bool m_Simulating = false;
    bool m_Paused = false;
    bool m_localSpace = false;

    bool m_clicking = false;

  private:
    bool m_DidSizeChangeLastFrame = false;
    ImVec2 m_LastViewportSize;
  };

} // namespace vrm
