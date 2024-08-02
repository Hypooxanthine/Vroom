#pragma once

namespace vrm
{

class Event;

class Layer
{
public:
    Layer() = default;
    virtual ~Layer() = default;

    void init();
    void end();
    void update(float dt);
    void render();
    void submitEvent(Event& e);

    inline void setShouldUpdate(bool shouldUpdate) { m_ShouldUpdate = shouldUpdate; }
    inline void setShouldRender(bool shouldRender) { m_ShouldRender = shouldRender; }
    inline void setShouldHandleEvents(bool shouldHandleEvents) { m_ShouldHandleEvents = shouldHandleEvents; }

    inline bool isUpdating() const { return m_ShouldUpdate; }
    inline bool isRendering() const { return m_ShouldRender; }
    inline bool isHandlingEvents() const { return m_ShouldHandleEvents; }

protected:
    virtual void onInit() {}
    virtual void onEnd() {}
    virtual void onUpdate(float dt) {}
    virtual void onRender() {}
    virtual void onEvent(Event& e) {}

private:
    bool m_ShouldUpdate = true;
    bool m_ShouldRender = true;
    bool m_ShouldHandleEvents = true;
};
    
} // namespace vr
