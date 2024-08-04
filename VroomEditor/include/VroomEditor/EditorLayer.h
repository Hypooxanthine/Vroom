#pragma once

#include <Vroom/Core/Layer.h>
#include <Vroom/Event/CustomEvent/CustomEventManager.h>

namespace vrm
{

class EditorLayer : public vrm::Layer
{
public:
    EditorLayer();
    ~EditorLayer();

protected:

    virtual void onInit();
    virtual void onEnd();
    virtual void onUpdate(float dt);
    virtual void onRender();
    virtual void onEvent(vrm::Event& e);

private:
    CustomEventManager m_CustomEventManager;

};

} // namespace vrm