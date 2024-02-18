#include "Vroom/Core/Application.h"

#include "Vroom/Core/Assert.h"

namespace vrm
{

Application::Application(int argc, char** argv) noexcept
{
    Log::Init();

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
        Event e;
        m_Window->pollEvent(e);

        m_Window->swapBuffers();
    }
}

} // namespace vrm
