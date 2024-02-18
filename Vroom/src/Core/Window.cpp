#include "Vroom/Core/Window.h"

#include "Vroom/Core/Assert.h"

namespace vrm
{

static vrm::Window* ACTIVE_WINDOW = nullptr;

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    VRM_ASSERT(ACTIVE_WINDOW != nullptr);
    ACTIVE_WINDOW->keyCallback(window, key, scancode, action, mods);
}

void glfwMouseCallback(GLFWwindow* window, int button, int action, int mods)
{
    VRM_ASSERT(ACTIVE_WINDOW != nullptr);
    ACTIVE_WINDOW->mouseCallback(window, button, action, mods);
}

void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    VRM_ASSERT(ACTIVE_WINDOW != nullptr);
    ACTIVE_WINDOW->scrollCallback(window, xoffset, yoffset);
}

Window::Window() noexcept
{

}

Window::Window(const std::string& windowTitle, uint32_t width, uint32_t height) noexcept
{
    create(windowTitle, width, height);
}

Window::~Window()
{
    if (m_Handle)
        glfwDestroyWindow(m_Handle);
}

bool Window::create(const std::string& windowTitle, uint32_t width, uint32_t height)
{
    if (m_Handle)
        glfwDestroyWindow(m_Handle);

    m_Handle = glfwCreateWindow(width, height, windowTitle.c_str(), nullptr, nullptr);

    if (!m_Handle) return false;

    glfwSetKeyCallback(m_Handle, glfwKeyCallback);
    glfwSetMouseButtonCallback(m_Handle, glfwMouseCallback);
    glfwSetScrollCallback(m_Handle, glfwScrollCallback);

    glfwMakeContextCurrent(m_Handle);
    ACTIVE_WINDOW = this;

    return true;
}

bool Window::requestedClose() const
{
    return glfwWindowShouldClose(m_Handle) != 0;
}

Event Window::pollEvent()
{
    glfwPollEvents();

    return Event();
}

void Window::swapBuffers()
{
    glfwSwapBuffers(m_Handle);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    LOG_TRACE("Window::keyCallback called.");
}

void Window::mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
    LOG_TRACE("Window::mouseCallback called.");
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    LOG_TRACE("Window::scrollCallback called.");
}

} // namespace vrm