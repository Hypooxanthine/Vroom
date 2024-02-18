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
     * @brief Displays on the window everything that has been drawn since the last buffer swapping.
     */
    void swapBuffers();

public:
    friend void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    friend void glfwMouseCallback(GLFWwindow* window, int button, int action, int mods);
    friend void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

private:
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void mouseCallback(GLFWwindow* window, int button, int action, int mods);
    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

private:
    GLFWwindow* m_Handle = nullptr;
    std::queue<Event> m_EventQueue;
};

} // namespace vrm
