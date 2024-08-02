#pragma once

namespace vrm
{

class Event;

class Layer
{
public:
    Layer() = default;
    virtual ~Layer() = default;

    virtual void onAdd() {}
    virtual void onRemove() {}
    virtual void onUpdate(float dt) {}
    virtual void onRender() {}
    virtual void onEvent(Event& e) {}
};
    
} // namespace vr
