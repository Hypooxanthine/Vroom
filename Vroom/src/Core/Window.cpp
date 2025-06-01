#include "Vroom/Core/Window.h"

#include "Vroom/Core/Assert.h"

#include "Vroom/Event/GLFWEventsConverter.h"

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

void glfwCursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    VRM_ASSERT(ACTIVE_WINDOW != nullptr);
    ACTIVE_WINDOW->mouseMovedCallback(xpos, ypos);
}

void glfwCursorEnterCallback(GLFWwindow* window, int entered)
{
    VRM_ASSERT(ACTIVE_WINDOW != nullptr);
    ACTIVE_WINDOW->mouseEnteredCallback(entered);
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

void glfwWindowCloseCallback(GLFWwindow* window)
{
    VRM_ASSERT(ACTIVE_WINDOW != nullptr);
    ACTIVE_WINDOW->closeCallback();
}

void glfwDragEnterCallback(GLFWwindow* window, int entered)
{
  VRM_ASSERT(ACTIVE_WINDOW != nullptr);
  ACTIVE_WINDOW->dragEnterCallback(entered);
}

void glfwDropCallback(GLFWwindow* window, int path_count, const char* paths[])
{
  VRM_ASSERT(ACTIVE_WINDOW != nullptr);
  std::string pathsStr;

  if (path_count > 0)
  {
    pathsStr += paths[0];
    
    for (int i = 1; i < path_count; ++i)
    {
      pathsStr += ';';
      pathsStr += paths[i];
    }
  }
  
  ACTIVE_WINDOW->dropCallback(std::move(pathsStr));
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

    if (!m_Handle)
    {
        const char* errorDesc;
        int errorCode = glfwGetError(&errorDesc);
        VRM_LOG_ERROR("Failed to create window. GLFW error code: {0:x}. Description: {1}.", errorCode, errorDesc);
        return false;
    }

    m_Title = windowTitle;
    m_Width = width;
    m_Height = height;

    glfwSetKeyCallback(m_Handle, glfwKeyCallback);
    glfwSetMouseButtonCallback(m_Handle, glfwMouseCallback);
    glfwSetCursorPosCallback(m_Handle, glfwCursorPosCallback);
    glfwSetCursorEnterCallback(m_Handle, glfwCursorEnterCallback);
    glfwSetScrollCallback(m_Handle, glfwScrollCallback);
    glfwSetWindowSizeCallback(m_Handle, glfwWindowSizeCallback);
    glfwSetWindowFocusCallback(m_Handle, glfwFocusedCallback);
    glfwSetWindowCloseCallback(m_Handle, glfwWindowCloseCallback);
    glfwSetDragEnterCallback(m_Handle, glfwDragEnterCallback);
    glfwSetDropCallback(m_Handle, glfwDropCallback);

    glfwMakeContextCurrent(m_Handle);
    ACTIVE_WINDOW = this;

    glfwSwapInterval(0);

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

bool Window::getCursorVisible() const
{
    return m_CursorVisible;
}

void Window::setCursorVisible(bool visible)
{
    if (m_CursorVisible == visible) return;

    m_CursorVisible = visible;
    
    // If cursor is visible, we need to update the last mouse position because mouse will suddenly appear at its last visible position
    if (visible)
    {
        glfwSetInputMode(m_Handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwGetCursorPos(m_Handle, &m_lastMouseX, &m_lastMouseY);
        glfwSetInputMode(m_Handle, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
    }
    else
    {
        glfwSetInputMode(m_Handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (glfwRawMouseMotionSupported())
            glfwSetInputMode(m_Handle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }
}

void Window::setKeyRepeatEnabled(bool keyRepeat)
{
    m_KeyRepeatEnabled = keyRepeat;
}

glm::ivec2 Window::getCursorPos() const
{
  return {
    static_cast<int>(m_lastMouseX),
    static_cast<int>(m_lastMouseY)
  };
}

bool Window::requestedClose() const
{
    return glfwWindowShouldClose(m_Handle) != 0;
}

void Window::requestClose()
{
    glfwSetWindowShouldClose(m_Handle, GLFW_TRUE);
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
    if (action == GLFW_REPEAT && !m_KeyRepeatEnabled) return;
    
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

void Window::mouseMovedCallback(double xpos, double ypos)
{
    Event& e = m_EventQueue.emplace();
    e.type = Event::Type::MouseMoved;
    e.mouseX = xpos;
    e.mouseY = ypos;
    e.mouseDeltaX = xpos - m_lastMouseX;
    e.mouseDeltaY = ypos - m_lastMouseY;

    m_lastMouseX = xpos;
    m_lastMouseY = ypos;
}

void Window::mouseEnteredCallback(int entered)
{
    Event& e = m_EventQueue.emplace();
    e.type = (entered == GLFW_TRUE ? Event::Type::MouseEntered : Event::Type::MouseLeft);
    glfwGetCursorPos(m_Handle, &m_lastMouseX, &m_lastMouseY);
    e.mouseX = m_lastMouseX;
    e.mouseY = m_lastMouseY;
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
    if (m_Width == width && m_Height == height) return;

    m_Width = width;
    m_Height = height;

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

void vrm::Window::closeCallback()
{
    Event& e = m_EventQueue.emplace();
    e.type = Event::Type::Exit;
}

void Window::dragEnterCallback(int entered)
{
  Event& e = m_EventQueue.emplace();
  e.type = (entered == GLFW_TRUE ? Event::Type::FileDragEnter : Event::Type::FileDragLeave);
}

void Window::dropCallback(std::string&& filePaths)
{
  Event& e = m_EventQueue.emplace();
  e.type = Event::Type::FileDrop;
  e.stringData = std::move(filePaths);
}

} // namespace vrm

