#pragma once

namespace vrm
{

struct Event;

/**
 * @brief Layer class. Layers are used to separate different parts of the application. Layers are rendered in the order they are added, and updated in the reverse order.
 * 
 */
class Layer
{
public:
    Layer() = default;
    virtual ~Layer() = default;

    /**
     * @brief Initialize the layer.
     * 
     */
    void init();

    /**
     * @brief End the layer.
     * 
     */
    void end();

    /**
     * @brief Notify the layer that a new frame has started.
     * 
     */
    void newFrame();

    /**
     * @brief Update the layer.
     * 
     * @param dt Delta time.
     */
    void update(float dt);

    /**
     * @brief Render the layer.
     * 
     */
    void render();

    /**
     * @brief Submit an event to the layer.
     * 
     * @param e Event to submit.
     */
    void submitEvent(Event& e);

    /**
     * @brief Set if the layer should update.
     * 
     * @param shouldUpdate Should update.
     */
    inline void setShouldUpdate(bool shouldUpdate) { m_ShouldUpdate = shouldUpdate; }

    /**
     * @brief Set if the layer should render.
     * 
     * @param shouldRender Should render.
     */
    inline void setShouldRender(bool shouldRender) { m_ShouldRender = shouldRender; }

    /**
     * @brief Set if the layer should handle events.
     * 
     * @param shouldHandleEvents Should handle events.
     */
    inline void setShouldHandleEvents(bool shouldHandleEvents) { m_ShouldHandleEvents = shouldHandleEvents; }

    /**
     * @brief Check if the layer is updating.
     * 
     * @return true If the layer is updating.
     */
    inline bool isUpdating() const { return m_ShouldUpdate; }

    /**
     * @brief Check if the layer is rendering.
     * 
     * @return true If the layer is rendering.
     */
    inline bool isRendering() const { return m_ShouldRender; }

    /**
     * @brief Check if the layer is handling events.
     * 
     * @return true If the layer is handling events.
     */
    inline bool isHandlingEvents() const { return m_ShouldHandleEvents; }

protected:

    /**
     * @brief Override this function to add custom initialization behavior.
     * 
     */
    virtual void onInit() {}

    /**
     * @brief Override this function to add custom end behavior.
     * 
     */
    virtual void onEnd() {}

    /**
     * @brief Override this function to add custom behavior when a new frame starts.
     * 
     */
    virtual void onNewFrame() {}

    /**
     * @brief Override this function to add custom update behavior.
     * 
     * @param dt Delta time.
     */
    virtual void onUpdate(float dt) {}

    /**
     * @brief Override this function to add custom render behavior.
     * 
     */
    virtual void onRender() {}

    /**
     * @brief Override this function to add custom event handling behavior.
     * 
     * @param e Event to handle.
     */
    virtual void onEvent(Event& e) {}

    /**
     * @brief Forces the update of the layer on next frame, even if layer update is turned off.
     * 
     */
    void forceUpdateNextFrame();

    /**
     * @brief Forces the render of the layer on next frame, even if layer rendering is turned off.
     * 
     * 
     */
    void forceRenderNextFrame();

private:
    bool m_ShouldUpdate = true;
    bool m_ForceUpdateNextFrame = false;
    bool m_ShouldRender = true;
    bool m_ForceRenderNextFrame = false;
    bool m_ShouldHandleEvents = true;
};
    
} // namespace vr
