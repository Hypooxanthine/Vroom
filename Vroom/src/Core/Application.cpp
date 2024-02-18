#include "Vroom/Core/Application.h"

#include "Vroom/Core/Assert.h"
#include "Vroom/Event/GLFWEventsConverter.h"

namespace vrm
{

Application::Application(int argc, char** argv)
{
    Log::Init();
    GLFWEventsConverter::Init();

    VRM_ASSERT(initGLFW());
    m_Window = std::make_unique<Window>("Vroom engine", 600, 400);

    VRM_ASSERT(glewInit() == GLEW_OK);

    LOG_TRACE("Vroom application created.");
}

Application::~Application()
{
    glfwTerminate();
}

bool Application::initGLFW()
{
    if (!glfwInit()) return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

    return true;
}

void Application::run()
{
    while (!m_Window->requestedClose())
    {
        m_Window->updateEvents();
        while (m_Window->hasPendingEvents())
        {
            Event e = m_Window->pollEvent();

            std::string type;
            if (e.keyEvent)
            {
                type = "KeyEvent";
                if (e.keyPressed)
                    type += " - pressed";
                else
                    type += " - released";
            }
            if (e.mouseEvent)
            {
                type = "MouseEvent";
                if (e.mouseButtonPressed)
                    type += " - pressed";
                else
                    type += " - released";
            }
            if (e.scrollEvent)
            {
                type = "ScrollEvent - x:" + std::to_string(e.scrollX) + ", y:" + std::to_string(e.scrollY);
            }

            LOG_TRACE("Triggered event of type {}.", type);

        }

        m_Window->swapBuffers();
    }
}

} // namespace vrm
