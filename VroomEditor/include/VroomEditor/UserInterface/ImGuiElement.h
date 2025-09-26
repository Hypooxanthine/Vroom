#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS

#include "Vroom/Core/DeltaTime.h"
namespace vrm
{

class ImGuiElement
{
public:

  ImGuiElement();
  virtual ~ImGuiElement();

  inline void update(const DeltaTime& dt) { onUpdate(dt); }
  inline void render() { onRender(); }

  void renderImgui();

  inline void setOpenBoolean(bool* open) { m_open = open; }
  inline void resetOpenBoolean() { setOpenBoolean(nullptr); }

protected:

  virtual void onUpdate(const DeltaTime& dt) {}
  virtual void onRender() {}
  virtual void onImgui() = 0;
  void         notImplemented();

  bool* m_open = nullptr;
};

} // namespace vrm
