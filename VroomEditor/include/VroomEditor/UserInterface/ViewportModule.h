#pragma once

#include "Vroom/Render/RenderView.h"
#include "VroomEditor/UserInterface/ImGuiElement.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>

namespace vrm
{

  namespace gl
  {
    class Texture;
  }

  class ViewportModule : public ImGuiElement
  {
  public:

    ViewportModule();
    ~ViewportModule();

    ViewportModule& operator=(const ViewportModule& other) = delete;
    ViewportModule(const ViewportModule& other) = delete;

    ViewportModule& operator=(ViewportModule&& other) = delete;
    ViewportModule(ViewportModule&& other) = delete;

    void setSupportPopup(bool val);
    void setSupportGuizmo(bool val, const render::NormalizedViewport& viewport = { 0.f, 0.f, 1.f, 1.f });
    inline void setTexture(const gl::Texture* texture) { m_texture = texture; }

    // If mouse is pressed but not moving, calling this function will allow dragging to start
    void allowDraggingIfMousePressed();

    inline const ImVec2& getSize() const { return m_size; }
    inline bool isLocalSpace() const { return m_localSpace; }
    inline bool isDraggingLeft() const { return m_lmbDragging; } 

  protected:

    void onImgui() override;

    void closePopup();

    virtual void onPopupImgui(const ImVec2& texturePx) {}

    virtual void onLeftClick(const ImVec2& texturePx) {}
    virtual void onDrag(ImGuiMouseButton button, const ImVec2& texturePx) {}
    virtual void onResize(const ImVec2& textureSize) {}

  private:

    const gl::Texture* m_texture = nullptr;
    ImVec2 m_size;

    bool m_supportPopup = true;
    bool m_supportGuizmo = false;
    bool m_localSpace = false;
    render::NormalizedViewport m_guizmoViewport = { 0.f, 0.f, 1.f, 1.f };
    bool m_clicking = false;
    bool m_lmbDragging = false;
  };

}