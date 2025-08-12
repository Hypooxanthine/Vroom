#pragma once

namespace vrm
{

class ImGuiElement
{
public:
  ImGuiElement();
  virtual ~ImGuiElement();

  bool renderImgui();

  inline void setOpenBoolean(bool* open) { m_open = open; }
  inline void resetOpenBoolean() { setOpenBoolean(nullptr); }

protected:
  virtual bool onImgui() = 0;
  void notImplemented();

  bool* m_open = nullptr;

};

} // namespace vrm