#pragma once

#include <future>
#include "Editor/ImGuiElement.h"

namespace vrm
{

class MainMenuBar : public ImGuiElement
{
public:
  MainMenuBar();
  ~MainMenuBar();

protected:
  void onImgui() override;
  void onUpdate(const DeltaTime& dt) override;

private:

  std::future<bool> m_buildScriptRet = {};

};

} // namespace vrm
