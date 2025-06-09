#pragma once

#include <memory>
#include <chrono>
#include <stack>

#include <GL/glew.h>

#include "Vroom/Core/LayerStack.h"
#include "Vroom/Core/DeltaTime.h"

namespace vrm
{

class Window;
class Layer;
class GameLayer;

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

    /**
     * @brief Push a layer to the application.
     * 
     * @tparam L The type of the layer.
     * @tparam Args The types of the arguments to pass to the layer constructor.
     * @param args The arguments to pass to the layer constructor.
     */
    template <typename L, typename... Args>
    L& pushLayer(Args&&... args)
    {
        std::unique_ptr<L> layer;
        layer = std::make_unique<L>(std::forward<Args>(args)...);
        L* layerPtr = layer.get();

        m_LayerStack.push(std::move(layer));

        return *layerPtr;
    }

    /**
     * @brief Get the game layer.
     * 
     * @return GameLayer& The game layer.
     */
    inline GameLayer& getGameLayer() { return *m_GameLayer; }

    /**
     * @brief Get the game layer.
     * 
     * @return GameLayer& The game layer object.
     */
    inline const GameLayer& getGameLayer() const { return *m_GameLayer; }

    /**
     * @brief Get the window.
     * 
     * @return Window& The window object.
     */
    inline Window& getWindow() { return *m_Window; }

    /**
     * @brief Get the window.
     * 
     * @return const Window& The window object.
     */
    inline const Window& getWindow() const { return *m_Window; }

    /**
     * @brief Get the delta time.
     * 
     * @return float The delta time.
     */
    inline const DeltaTime& getDeltaTime() const { return m_DeltaTime; }

    void setFrameRateLimit(uint16_t frameRate);

    inline uint16_t getFrameRateLimit() const { return m_frameRateLimit; }

private:
    
    /**
     * @brief Initialize GLFW.
     * 
     * @return true If GLFW was initialized successfully.
     * @return false Otherwise.
     */
    bool initGLFW();

    /**
     * @brief Initialize the layers.
     * 
     */
    void initLayers();

    /**
     * @brief First step of the application loop.
     * 
     */
    void newFrame();

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

private:
    static Application* s_Instance;

    std::unique_ptr<Window> m_Window;

    // Layers
    LayerStack m_LayerStack;
    GameLayer* m_GameLayer;

    std::chrono::high_resolution_clock::time_point m_LastFrameTimePoint;
    DeltaTime m_DeltaTime;
    
    uint16_t m_frameRateLimit = 0;
    uint64_t m_minFrameTimeNanoseconds = 0;
    float m_timeSinceLastFrame = 0.f;

    bool m_PendingKilled = false;

};

} // namespace vrm
