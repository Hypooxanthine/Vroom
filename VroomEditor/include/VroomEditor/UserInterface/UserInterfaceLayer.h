#pragma once

#include <memory>
#include <list>

#include <Vroom/Render/Abstraction/FrameBuffer.h>

#include "VroomEditor/UserInterface/ImGuiElement.h"
#include "Vroom/Core/Layer.h"

struct ImFont;

namespace vrm
{

class UserInterfaceLayer : public Layer
{
public:
  UserInterfaceLayer();
  virtual ~UserInterfaceLayer();

  static UserInterfaceLayer& Get();

  void onViewportResize(int newWidth, int newHeight);
  void onViewportActive(bool active);
  void onViewportSimulating(bool simulating);
  void onViewportPaused(bool paused);

  inline const auto& getViewportInfo() const { return m_ViewportInfo; }

protected:
  virtual void onInit() override;
  virtual void onEnd() override;
  virtual void onUpdate(float dt) override;
  virtual void onRender() override;
  virtual void onEvent(vrm::Event &e) override;

private:
  template <typename T, typename... Args>
  T& emplaceImGuiElement(Args&&... args)
  {
    auto& uniquePtr = m_Elements.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
    return static_cast<T&>(*uniquePtr.get());
  }

  void resetUIInfos();
  void renderImgui();

private:
  ImFont *m_Font;

  std::list<std::unique_ptr<ImGuiElement>> m_Elements;

  struct ViewportInfos
  {
    bool justChangedSize = false;
    int width;
    int height;
    bool active = false;
    bool simulating = false;
    bool paused = false;
  } m_ViewportInfo;
};

} // namespace vrm
