#pragma once

#include <Vroom/Scene/Components/ScriptComponent.h>

class SuzanneScript : public vrm::ScriptComponent
{
public:
    SuzanneScript(float circleRadius, float startAngle, float speed);
    ~SuzanneScript() = default;

    void onUpdate(float dt) override;

private:
    float m_CircleRadius;
    float m_Angle;
    float m_Speed;
};