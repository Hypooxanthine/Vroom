#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string>
#include <queue>

#include "Vroom/Event/Event.h"

namespace vrm
{

/**
 * @brief Window class uses glfw and expects glfw to be initialized. Does not terminate glfw neither.
 * 
 */
class Window
{
public:
    /**
     * @brief Constructs an empty Window object. Call Window::create() for opening it.
     */
    Window();

    /**
     * @brief Constructs a Window object and opens it.
     * @param windowTitle Title of the window.
     * @param width Width of the window (in pixels).
     * @param height Height of the window (in pixels).
     */
    Window(const std::string& windowTitle, uint32_t width, uint32_t height);

    /**
     * @brief Destroys the window instance in the operating system.
     */
    ~Window();

    /**
     * @brief Destroys the window if it has already been created, then creates it (again) and opens it.
     * @param windowTitle Title of the window.
     * @param width Width of the window (in pixels).
     * @param height Height of the window (in pixels).
     * @return True if succeded. False otherwise.
     */
    bool create(const std::string& windowTitle, uint32_t width, uint32_t height);

    /**
     * @brief Destroys the window.
     */
    void destroy();

    /**
     * @brief Gets window title.
     * @return Window title.
     */
    const std::string& getTitle() const;

    /**
     * @brief Gets window width.
     * @return Window width (in pixels).
     */
    int getWidth() const;

    /**
     * @brief Gets window height.
     * @return Window height (in pixels).
     */
    int getHeight() const;

    /**
     * @brief Get the Key Repeat value.
     * 
     * @return true Key repeat is enabled.
     * @return false Key repeat is disabled.
     */
    bool getKeyRepeatEnabled() const;

    /**
     * @brief Set the Key Repeat value. If true, key repeat events will be treated as key pressed. If false, key repeat events will be ignored.
     * 
     * @param keyRepeat Key repeat value.
     */
    void setKeyRepeatEnabled(bool keyRepeat);

    /**
     * @brief Checks if user requested the window closing.
     * @return True if requested. False otherwise.
     */
    bool requestedClose() const;

    /**
     * @brief Request window closing.
     */
    void requestClose();

    /**
     * @brief Starts a new frame for events. It will reset the events queue and get all triggered events since the last call of this function.
     */
    void updateEvents();

    /**
     * @brief Checks if there is at least one pending event in the queue.
     * @return True if there is a pending event. False otherwise.
     */
    bool hasPendingEvents() const;

    /**
     * @brief Gets the next event from the queue.
     * When user triggers an event, it is placed into a FIFO queue. The function returns the next event from this queue
     * then removes it from it.
     * You should call Window::hasPendingEvents() before polling an event.
     * @return The next event from the events queue.
     */
    Event pollEvent();

    /**
     * @brief Check if a key is currently pressed.
     * @param code Key to be checked.
     * @return True if pressed. False otherwise.
     */
    bool isKeyPressed(const KeyCode& code) const;

    /**
     * @brief Check if a mouse button is currently pressed.
     * @param code Mouse button to be checked.
     * @return True if pressed. False otherwise.
     */
    bool isMousePressed(const MouseCode& code) const;

    /**
     * @brief Displays on the window everything that has been drawn since the last buffer swapping.
     */
    void swapBuffers();

public:
    friend void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    friend void glfwMouseCallback(GLFWwindow* window, int button, int action, int mods);
    friend void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    friend void glfwWindowSizeCallback(GLFWwindow* window, int width, int height);
    friend void glfwFocusedCallback(GLFWwindow* window, int focused);
    friend void glfwWindowCloseCallback(GLFWwindow* window);

private:

    void keyCallback(int key, int scancode, int action, int mods);
    void mouseCallback(int button, int action, int mods);
    void scrollCallback(double xoffset, double yoffset);
    void resizeCallback(int width, int height);
    void focusCallback(int focused);
    void closeCallback();

private:
    std::string m_Title;
    int m_Width = 0, m_Height = 0;
    GLFWwindow* m_Handle = nullptr;
    std::queue<Event> m_EventQueue;

    bool m_KeyRepeatEnabled = false;
};

} // namespace vrm
