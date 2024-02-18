#include "Vroom/Core/Window.h"

#include "Vroom/Core/Assert.h"

#include "Vroom/Event/GLFWEventsConverter.h"

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

void Window::updateEvents()
{
    // Clearing the queue
    std::queue<Event> empty;
    std::swap(m_EventQueue, empty);

    glfwPollEvents();
}

bool Window::hasPendingEvents() const
{
    return !m_EventQueue.empty();
}

Event Window::pollEvent()
{
    Event e = m_EventQueue.front();
    m_EventQueue.pop();

    return e;
}

void Window::swapBuffers()
{
    glfwSwapBuffers(m_Handle);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Event e;
    e.keyCode = GLFWEventsConverter::GetKeyCodeFromGLFW(key);
    e.keyEvent = true;
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
        e.keyPressed = true;
    else
        e.keyReleased = true;

    m_EventQueue.push(e);
}

void Window::mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
    Event e;
    e.mouseCode = GLFWEventsConverter::GetMouseCodeFromGLFW(button);
    e.mouseEvent = true;
    if (action == GLFW_PRESS)
        e.mouseButtonPressed = true;
    else
        e.mouseButtonReleased = true;

    m_EventQueue.push(e);
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    Event e;
    e.scrollEvent = true;
    e.scrollX = xoffset;
    e.scrollY = yoffset;

    m_EventQueue.push(e);
}

} // namespace vrm