#pragma once

#include <memory>

#include <GL/glew.h>

#include "Vroom/Core/Window.h"
#include "Vroom/Render/Renderer.h"
#include "Vroom/Event/Trigger/TriggerManager.h"
#include "Vroom/Event/Trigger/TriggerBinder.h"

namespace vrm
{

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

    TriggerBinder createTrigger(const std::string& name);

private:
    /**
     * @brief Initialization of GLFW.
     * @return True if succeeded. False otherwise.
     */
    bool initGLFW();

    void update();

    void draw();

private:
    std::unique_ptr<Window> m_Window = nullptr;
    std::unique_ptr<Renderer> m_Renderer = nullptr;

    TriggerManager m_TriggerManager;
    bool m_PendingKilled = false;
};

} // namespace vrm
