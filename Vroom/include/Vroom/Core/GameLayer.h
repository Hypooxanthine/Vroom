#pragma once

#include "Vroom/Core/Layer.h"

namespace vrm
{

class GameLayer : public Layer
{
public:
    GameLayer() = default;
    ~GameLayer() = default;

    void onAdd() override;
    void onRemove() override;
    void onUpdate(float dt) override;
    void onRender() override;
    void onEvent(Event& e) override;
};

} // namespace vr