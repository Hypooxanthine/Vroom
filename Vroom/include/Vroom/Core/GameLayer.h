#pragma once

#include "Vroom/Core/Layer.h"

namespace vrm
{

class GameLayer : public Layer
{
public:
    GameLayer() = default;
    ~GameLayer() = default;

protected:
    void onInit() override;
    void onEnd() override;
    void onUpdate(float dt) override;
    void onRender() override;
    void onEvent(Event& e) override;

private:

};

} // namespace vr