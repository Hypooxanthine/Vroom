#pragma once

#include <memory>

#include "Vroom/Core/Layer.h"

#include "Vroom/Render/Abstraction/FrameBuffer.h"
#include "Vroom/Event/Trigger/TriggerManager.h"
#include "Vroom/Event/CustomEvent/CustomEventManager.h"

namespace vrm
{

class Scene;

/**
 * @brief The game layer of the application.
 *
 */
class GameLayer : public Layer
{
public:
  GameLayer();
  ~GameLayer();

  /**
   * @brief Loads a scene into the game layer. Scene will start at the beginning of the next frame.
   *
   * @tparam S Scene type. Must be a child of Scene type.
   * @tparam Args Argument types taken by the constructor of S that needs to be called.
   * @param args Arguments for constructing an instance of S.
   */
  template <typename S, typename... Args>
  void loadScene(Args &&...args)
  {
    std::unique_ptr<S> scene;
    scene.reset(new S(std::forward<Args>(args)...));

    loadScene(std::move(scene));
  }

  void loadScene(std::unique_ptr<Scene> &&scene)
  {
    loadScene_Internal(std::move(scene));
  }

  /**
   * @brief Get the loaded scene.
   *
   * @return const Scene& The loaded scene.
   */
  inline const Scene &getScene() const { return *m_CurrentScene; }

  /**
   * @brief Get the loaded scene.
   *
   * @return Scene& The loaded scene.
   */
  inline Scene &getScene() { return *m_CurrentScene; }

  /**
   * @brief Create a trigger.
   *
   * @param triggerName The name of the trigger.
   * @return TriggerBinder The binder of the trigger. Handful for binding inputs and callbacks to a trigger after its creation.
   * @see @ref triggers
   */
  TriggerBinder createTrigger(const std::string &triggerName);

  /**
   * @brief Get the trigger binder of a specific trigger.
   *
   * @param triggerName The name of the trigger.
   * @return TriggerBinder The binder of the trigger. Handful for binding inputs and callbacks to a trigger after its creation.
   */
  TriggerBinder getTrigger(const std::string &triggerName);

  /**
   * @brief Create a custom event.
   *
   * @param customEventName The name of the custom event.
   * @return CustomEventBinder The binder of the trigger. Handful for binding inputs and callbacks to a custom event after its creation.
   * @see @ref custom_events
   */
  CustomEventBinder createCustomEvent(const std::string &customEventName);

  /**
   * @brief Get the custom event binder of a specific custom event.
   *
   * @param customEventName The name of the trigger.
   * @return CustomEventBinder The binder of the trigger. Handful for binding inputs and callbacks to a custom event after its creation.
   * @see @ref custom_events
   */
  CustomEventBinder getCustomEvent(const std::string &customEventName);

private:
  /**
   * @brief Load a scene into the application.
   *
   * @param scene The scene to load.
   */
  void loadScene_Internal(std::unique_ptr<Scene> &&scene);

  /**
   * @brief Check if the next scene is ready to be loaded.
   *
   */
  void checkNextScene();

protected:
  void onInit() override;
  void onEnd() override;
  void onNewFrame() override;
  void onUpdate(float dt) override;
  void onRender() override;
  void onEvent(Event &e) override;

private:
  std::unique_ptr<Scene> m_CurrentScene, m_NextScene;

  TriggerManager m_TriggerManager;
  CustomEventManager m_CustomEventManager;
};

} // namespace vr