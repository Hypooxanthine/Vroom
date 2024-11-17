#pragma once

#include <Vroom/Scene/Components/ScriptComponent.h>

class SuzanneScript : public vrm::ScriptComponent
{
public:
    SuzanneScript(float circleRadius, float startAngle, float speed);
    ~SuzanneScript() = default;

    void onSpawn() override;

    void onUpdate(float dt) override;

private:
    void updatePosition(float dt);

private:
    float m_CircleRadius;
    float m_Angle;
    float m_Speed;
};