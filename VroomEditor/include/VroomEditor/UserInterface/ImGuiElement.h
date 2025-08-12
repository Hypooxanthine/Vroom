#pragma once

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

  bool renderImgui();

  inline void setOpenBoolean(bool* open) { m_open = open; }
  inline void resetOpenBoolean() { setOpenBoolean(nullptr); }

protected:
  virtual void onUpdate(const DeltaTime& dt) {}
  virtual void onRender() {}
  virtual bool onImgui() = 0;
  void notImplemented();

  bool* m_open = nullptr;

};

} // namespace vrm