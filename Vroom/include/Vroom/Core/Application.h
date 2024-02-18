#pragma once

#include <memory>

#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "Vroom/Core/Window.h"

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
    Application(int argc, char** argv) noexcept;
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

private:
    /**
     * @brief Initialization of GLFW.
     * @return True if succeeded. False otherwise.
     */
    bool initGLFW();

private:
    std::unique_ptr<Window> m_Window = nullptr;
};

} // namespace vrm
