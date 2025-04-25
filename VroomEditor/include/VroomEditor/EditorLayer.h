#pragma once

#include <Vroom/Core/Layer.h>
#include <Vroom/Event/CustomEvent/CustomEventManager.h>
#include <Vroom/Event/Trigger/TriggerManager.h>
#include <Vroom/Render/Camera/FirstPersonCamera.h>

#include "VroomEditor/EditorCamera.h"

namespace vrm
{

class UserInterfaceLayer;
class Scene;

class EditorLayer : public Layer
{
public:
  EditorLayer();
  ~EditorLayer();

  static EditorLayer& Get();

  template <typename S, typename... Args>
  void loadScene(Args&&... args)
  {
    auto scene = std::make_unique<S>(std::forward<Args>(args)...);
    loadScene(std::move(scene));
  }

  void loadScene(std::unique_ptr<Scene>&& scene);
  void loadScene(const std::string& sceneAssetName);

  void unloadScene();
  void saveScene();

protected:
  virtual void onInit() override;
  virtual void onEnd() override;
  virtual void onUpdate(float dt) override;
  virtual void onRender() override;
  virtual void onEvent(vrm::Event &e) override;

private:
  void onViewportResize(int newWidth, int newHeight);

private:
  CustomEventManager m_CustomEventManager;
  TriggerManager m_TriggerManager;
  bool m_SceneLoaded = false;

  EditorCamera m_EditorCamera;
};

} // namespace vrm