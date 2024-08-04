#pragma once

#include <Vroom/Scene/Scene.h>

namespace vrm
{

class EditorScene : public vrm::Scene
{
public:
    EditorScene() = default;
    ~EditorScene() = default;

protected:
    void onInit() override;
    void onUpdate(float deltaTime) override;
    void onRender() override;
    void onEnd() override;
};

} // namespace vrm