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

Window::Window()
{

}

Window::Window(const std::string& windowTitle, uint32_t width, uint32_t height)
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
    destroy();

    m_Handle = glfwCreateWindow(width, height, windowTitle.c_str(), nullptr, nullptr);

    if (!m_Handle) return false;

    m_Title = windowTitle;
    m_Width = width;
    m_Height = height;

    glfwSetKeyCallback(m_Handle, glfwKeyCallback);
    glfwSetMouseButtonCallback(m_Handle, glfwMouseCallback);
    glfwSetScrollCallback(m_Handle, glfwScrollCallback);

    glfwMakeContextCurrent(m_Handle);
    ACTIVE_WINDOW = this;

    return true;
}

void Window::destroy()
{
    if (!m_Handle) return;

    glfwDestroyWindow(m_Handle);
    m_Handle = nullptr;
    m_Title = "";
    m_Width = 0;
    m_Height = 0;
}

const std::string& Window::getTitle() const
{
    return m_Title;
}

int Window::getWidth() const
{
    return m_Width;
}

int Window::getHeight() const
{
    return m_Height;
}

bool Window::requestedClose() const
{
    return glfwWindowShouldClose(m_Handle) != 0;
}

void Window::requestClose()
{
    glfwSetWindowShouldClose(m_Handle, true);
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

bool Window::isKeyPressed(const KeyCode& code) const
{
    int GLFWCode = GLFWEventsConverter::GetGLFWFromKeyCode(code);
    return glfwGetKey(m_Handle, GLFWCode) == GLFW_PRESS ? true : false;
}

bool Window::isMousePressed(const MouseCode& code) const
{
    int GLFWCode = GLFWEventsConverter::GetGLFWFromMouseCode(code);
    return glfwGetMouseButton(m_Handle, GLFWCode) == GLFW_PRESS ? true : false;
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