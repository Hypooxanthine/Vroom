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

    /**
     * @brief Create an event trigger.
     * A trigger is a particular type of event that can be activated and deactivated.
     * For this reason, it only works with inputs that can be pressed (activation) and released (deactivation).
     * A trigger can be bound with multiple inputs and callbacks.
     * For example, a trigger could be "MoveForward". When keys Q or Up arrow are pressed, trigger is activated.
     * When trigger was activated, and Q and Up arrow aren't pressed anymore, trigger is unactivated.
     * When a trigger is activated AND deactivated, all associated callbacks are called with a boolean value 
     * (activated / deactivated).
     * 
     * @param name Trigger name
     * @return TriggerBinder A facility class instance that helps binding multiple keys and callbacks for the same trigger.
     */
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
