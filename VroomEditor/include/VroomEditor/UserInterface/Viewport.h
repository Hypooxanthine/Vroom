#pragma once

#include "VroomEditor/UserInterface/ImGuiElement.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"

#include <glm/fwd.hpp>

#include <Vroom/Scene/Entity.h>
#include "VroomEditor/UserInterface/ViewportModule.h"

namespace vrm
{

  namespace gl
  {
    class Texture;
  }

  class Viewport : public ImGuiElement
  {
  private:

    class MyViewportModule : public ViewportModule
    {
    protected:
      void onPopupImgui(const ImVec2& texturePx) override;
      void onLeftClick(const ImVec2& texturePx) override;
      void onResize(const ImVec2& size) override;

    private:
      Entity m_cachedPopupEntity;
    };

  public:
    Viewport();
    ~Viewport();

    inline void setIdle()
    {
      m_Playing = false;
      m_Simulating = false;
    }

    inline bool isPlaying() const { return m_Playing; }
    inline bool isSimulating() const { return m_Simulating; }
    inline bool isPaused() const { return m_Paused; }
    inline bool isIdle() const { return !m_Playing && !m_Simulating; }

    inline void setRenderTexture(const gl::Texture *tex) { m_viewportModule.setTexture(tex); }

    inline void allowActivation() { m_viewportModule.allowDraggingIfMousePressed(); }

  protected:
    void onImgui() override;

  private: // ImGui related variables

    bool m_Playing = false;
    bool m_Simulating = false;
    bool m_Paused = false;
    bool m_localSpace = false;

  private:
    MyViewportModule m_viewportModule;
  };

} // namespace vrm
