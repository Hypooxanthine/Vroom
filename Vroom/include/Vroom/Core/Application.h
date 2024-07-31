#pragma once

#include <memory>
#include <chrono>

#include <GL/glew.h>

#include "Vroom/Event/Trigger/TriggerManager.h"
#include "Vroom/Event/CustomEvent/CustomEventManager.h"

#include "Vroom/PublicInterfaces/WindowPublicInterface.h"

#include "Vroom/Asset/AssetManager.h"

#include "Vroom/Render/Abstraction/FrameBuffer.h"

namespace vrm
{

class Window;
class Renderer;
class Scene;

/**
 * @short The core class of the engine.
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
     * @brief Get the instance of the application.
     * 
     * @return Application& The instance of the application.
     */
    static Application& Get() { return *s_Instance; }

    /**
     * @brief Starts the application main loop.
     */
    void run();

    /**
     * @brief Requests application exit.
     * 
     */
    void exit();

    inline AssetManager& getAssetManager() { return *m_AssetManager; }
    inline const AssetManager& getAssetManager() const { return *m_AssetManager; }

    /**
     * @brief Create a trigger.
     * 
     * @param triggerName The name of the trigger.
     * @return TriggerBinder The binder of the trigger. Handful for binding inputs and callbacks to a trigger after its creation.
     * @see @ref triggers
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
     * 
     * @param customEventName The name of the custom event.
     * @return CustomEventBinder The binder of the trigger. Handful for binding inputs and callbacks to a custom event after its creation.
     * @see @ref custom_events
     */
    CustomEventBinder createCustomEvent(const std::string& customEventName);

    /**
     * @brief Get the custom event binder of a specific custom event.
     * 
     * @param customEventName The name of the trigger.
     * @return CustomEventBinder The binder of the trigger. Handful for binding inputs and callbacks to a custom event after its creation.
     * @see @ref custom_events
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

    /**
     * @brief Get the window object.
     * 
     * @return WindowPublicInterface The public interface of the window.
     */
    inline WindowPublicInterface getWindow() { return WindowPublicInterface(*m_Window); }

    /**
     * @brief Get the loaded scene.
     * 
     * @return const Scene& The loaded scene.
     */
    inline const Scene& getScene() const { return *m_CurrentScene; }

    /**
     * @brief Get the loaded scene.
     * 
     * @return Scene& The loaded scene.
     */
    inline Scene& getScene() { return *m_CurrentScene; }

    /**
     * @brief Get the renderer object.
     * 
     * @return Renderer& The renderer object.
     */
    inline const Renderer& getRenderer() const { return *m_Renderer; }

    /**
     * @brief Get the renderer object.
     * 
     * @return Renderer& The renderer object.
     */
    inline Renderer& getRenderer() { return *m_Renderer; }

    /**
     * @brief Get the frame buffer of the game.
     * 
     * @return const FrameBuffer& The frame buffer of the game.
     */
    inline FrameBuffer& getGameFrameBuffer() { return *m_GameFrameBuffer; }

    /**
     * @brief Get the frame buffer of the game.
     * 
     * @return const FrameBuffer& The frame buffer of the game.
     */
    inline const FrameBuffer& getGameFrameBuffer() const { return *m_GameFrameBuffer; }

private:
    
    /**
     * @brief Initialize GLFW.
     * 
     * @return true If GLFW was initialized successfully.
     * @return false Otherwise.
     */
    bool initGLFW();
    
    /**
     * @brief Check if the next scene is ready to be loaded.
     * 
     */
    void checkNextScene();

    /**
     * @brief Update step of the application.
     * 
     */
    void update();

    /**
     * @brief Draw step of the application.
     * 
     */
    void draw();

    /**
     * @brief Load a scene into the application.
     * 
     * @param scene The scene to load.
     */
    void loadScene_Internal(std::unique_ptr<Scene>&& scene);

private:
    static Application* s_Instance;

    std::unique_ptr<Window> m_Window;
    std::unique_ptr<Renderer> m_Renderer;
    std::unique_ptr<Scene> m_CurrentScene, m_NextScene;

    std::unique_ptr<AssetManager> m_AssetManager;

    TriggerManager m_TriggerManager;
    CustomEventManager m_CustomEventManager;

    std::chrono::high_resolution_clock::time_point m_LastFrameTimePoint;

    bool m_PendingKilled = false;

    std::unique_ptr<FrameBuffer> m_GameFrameBuffer;
};

} // namespace vrm
