#include "Vroom/Core/Window.h"

#include "Vroom/Core/Assert.h"

#include "Vroom/Event/GLFWEventsConverter.h"
#include "Window.h"

namespace vrm
{

static vrm::Window* ACTIVE_WINDOW = nullptr;

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    VRM_ASSERT(ACTIVE_WINDOW != nullptr);
    ACTIVE_WINDOW->keyCallback(key, scancode, action, mods);
}

void glfwMouseCallback(GLFWwindow* window, int button, int action, int mods)
{
    VRM_ASSERT(ACTIVE_WINDOW != nullptr);
    ACTIVE_WINDOW->mouseCallback(button, action, mods);
}

void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    VRM_ASSERT(ACTIVE_WINDOW != nullptr);
    ACTIVE_WINDOW->scrollCallback(xoffset, yoffset);
}

void glfwWindowSizeCallback(GLFWwindow* window, int width, int height)
{
    VRM_ASSERT(ACTIVE_WINDOW != nullptr);
    ACTIVE_WINDOW->resizeCallback(width, height);
}

void glfwFocusedCallback(GLFWwindow* window, int focused)
{
    VRM_ASSERT(ACTIVE_WINDOW != nullptr);
    ACTIVE_WINDOW->focusCallback(focused);
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
    glfwSetWindowSizeCallback(m_Handle, glfwWindowSizeCallback);
    glfwSetWindowFocusCallback(m_Handle, glfwFocusedCallback);

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

bool Window::getKeyRepeatEnabled() const
{
    return m_KeyRepeatEnabled;
}

void Window::setKeyRepeatEnabled(bool keyRepeat)
{
    m_KeyRepeatEnabled = keyRepeat;
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

void Window::keyCallback(int key, int scancode, int action, int mods)
{
    auto code = GLFWEventsConverter::GetKeyCodeFromGLFW(key);
    if (code == KeyCode::None) return;

    Event& e = m_EventQueue.emplace();
    e.keyCode = code;
    if (action == GLFW_PRESS || (m_KeyRepeatEnabled &&  action == GLFW_REPEAT))
        e.type = Event::Type::KeyPressed;
    else
        e.type = Event::Type::KeyReleased;
}

void Window::mouseCallback(int button, int action, int mods)
{
    auto code = GLFWEventsConverter::GetMouseCodeFromGLFW(button);
    if (code == MouseCode::None) return;

    Event& e = m_EventQueue.emplace();
    e.mouseCode = code;
    if (action == GLFW_PRESS)
        e.type = Event::Type::MousePressed;
    else
        e.type = Event::Type::MouseReleased;
}

void Window::scrollCallback(double xoffset, double yoffset)
{
    Event& e = m_EventQueue.emplace();
    e.type = Event::Type::Scroll;
    e.scrollX = xoffset;
    e.scrollY = yoffset;
}

void Window::resizeCallback(int width, int height)
{
    Event& e = m_EventQueue.emplace();
    e.type = Event::Type::WindowsResized;
    e.newWidth = width;
    e.newHeight = height;
}

void Window::focusCallback(int focused)
{
    Event& e = m_EventQueue.emplace();
    e.type = (focused == GLFW_TRUE ? Event::Type::GainedFocus : Event::Type::LostFocus);
}
} // namespace vrm