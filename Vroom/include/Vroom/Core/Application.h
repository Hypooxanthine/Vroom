#pragma once

#include <memory>
#include <chrono>

#include <GL/glew.h>

#include "Vroom/Event/Trigger/TriggerManager.h"
#include "Vroom/Event/CustomEvent/CustomEventManager.h"

namespace vrm
{

class Window;
class Renderer;
class Scene;

/**
 * @brief This is the core class of the engine. Everything related to the engine - including static calls - need to be done
 * AFTER creation of Application instance (it will initialize everything needed).
 * 
 */
class Application
{
public:

    /**
     * @brief Construct an Application object and intializes the engine.
     * 
     * @param argc Command line argc.
     * @param argv Command line argv.
     */
    Application(int argc, char** argv);
    ~Application();

    Application() = delete;
    Application(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

    /**
     * @brief Starts the application main loop.
     */
    void run();

    /**
     * @brief Requests application exit.
     * 
     */
    void exit();

    /**
     * @brief Create a trigger.
     * A trigger is a particular type of event that can be activated and deactivated.
     * For this reason, it only works with inputs that can be pressed (activation) and released (deactivation).
     * A trigger can be bound with multiple inputs and callbacks.
     * For example, a trigger could be "MoveForward". When keys Q or Up arrow are pressed, the trigger is activated.
     * When the trigger is activated, and then Q and Up arrow aren't pressed anymore, the trigger is unactivated.
     * Whenever a trigger is activated or deactivated, all associated callbacks are called with a boolean value 
     * (activated / deactivated).
     * 
     * @param triggerName The name of the trigger.
     * @return TriggerBinder The binder of the trigger. Handful for binding inputs and callbacks to a trigger after its creation.
     */
    TriggerBinder createTrigger(const std::string& triggerName);

    /**
     * @brief Get the trigger binder of a specific trigger.
     * 
     * @param triggerName The name of the trigger.
     * @return TriggerBinder The binder of the trigger. Handful for binding inputs and callbacks to a trigger after its creation.
     */
    TriggerBinder getTrigger(const std::string& triggerName);

    /**
     * @brief Create a custom event.
     * A custom event is a particular type of event that will be triggered when specific inputs are received.
     * When a custom event is triggered, all associated callbacks are triggered as well, given the raw event object
     * as a parameter.
     * A custom event can be bound with multiple inputs and callbacks.
     * Unlike triggers, custom events don't have on/off state. Thus, there is no limitation on event types, and you can
     * use any event type (they don't have to have a pressed/released state), such as vrm::Event::Type::Exit,
     * vrm::Event::Type::Scroll, ...
     * 
     * @param customEventName The name of the custom event.
     * @return CustomEventBinder The binder of the trigger. Handful for binding inputs and callbacks to a custom event after its creation.
     */
    CustomEventBinder createCustomEvent(const std::string& customEventName);

    /**
     * @brief Get the custom event binder of a specific custom event.
     * 
     * @param customEventName The name of the trigger.
     * @return CustomEventBinder The binder of the trigger. Handful for binding inputs and callbacks to a custom event after its creation.
     */
    CustomEventBinder getCustomEvent(const std::string& customEventName);

    /**
     * @brief Loads a scene into the application. Scene will start at the beginning of the next frame.
     * 
     * @tparam S Scene type. Must be a child of Scene type.
     * @tparam Args Argument types taken by the constructor of S that needs to be called.
     * @param args Arguments for constructing an instance of S.
     */
    template <typename S, typename... Args>
    void loadScene(Args&&... args)
    {
        std::unique_ptr<S> scene;
        scene = std::make_unique<S>(std::forward<Args>(args)...);

        loadScene_Internal(std::move(scene));
    }

private:
    /**
     * @brief Initialization of GLFW.
     * @return True if succeeded. False otherwise.
     */
    bool initGLFW();
    
    void checkNextScene();

    void update();

    void draw();
    
    void loadScene_Internal(std::unique_ptr<Scene>&& scene);

private:
    std::unique_ptr<Window> m_Window;
    std::unique_ptr<Renderer> m_Renderer;
    std::unique_ptr<Scene> m_CurrentScene, m_NextScene;

    TriggerManager m_TriggerManager;
    CustomEventManager m_CustomEventManager;

    std::chrono::high_resolution_clock::time_point m_LastFrameTimePoint;

    bool m_PendingKilled = false;
};

} // namespace vrm
