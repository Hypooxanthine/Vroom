#pragma once

#include <Application/Layer.h>
#include <Event/CustomEventManager.h>
#include <Event/TriggerManager.h>
#include <Renderer/FirstPersonCamera.h>
#include <future>

#include "Editor/Api.h"
#include "Editor/EditorCamera.h"
#include "Editor/UserInterfaceLayer.h"

namespace vrm
{

class UserInterfaceLayer;
class Scene;

class VRM_EDITOR_API EditorLayer : public Layer
{
public:

  EditorLayer();
  ~EditorLayer();

  static EditorLayer& Get();

  template <typename S, typename... Args>
  void loadScene(const std::string& name, Args&&... args)
  {
    m_loadedScene = name;
    auto scene    = std::make_unique<S>(std::forward<Args>(args)...);
    loadScene(name, std::move(scene));
  }

  void loadScene(const std::string& name, std::unique_ptr<Scene>&& scene);
  void loadScene(const std::string& sceneAssetName);

  inline bool isSceneLoaded() const
  {
    return m_loadedScene.size() > 0;
  }
  void unloadScene(bool force = false);
  void saveScene();

  void importFile(const std::filesystem::path& file);

  [[nodiscard]] std::future<bool> buildScriptsAsync();
  void                            reloadScripts();
  [[nodiscard]] std::future<bool> buildAndReloadAsync();

  inline const CameraBasic& getCurrentCamera() const
  {
    return m_EditorCamera;
  }

protected:

  virtual void onInit() override;
  virtual void onEnd() override;
  virtual void onUpdate(const DeltaTime& dt) override;
  virtual void onRender() override;
  virtual void onEvent(vrm::Event& e) override;

private:

  void onViewportResize(int newWidth, int newHeight);

  void _loadScriptsRuntimeLibrary();

private:

  CustomEventManager m_CustomEventManager;
  TriggerManager     m_TriggerManager;
  std::string        m_loadedScene = "";

  EditorCamera m_EditorCamera;

  UserInterfaceLayer::ViewportInfos m_lastViewportInfos;
};

} // namespace vrm
