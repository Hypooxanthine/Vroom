#pragma once

#include <Vroom/Scene/Components/ScriptComponent.h>

class MovingLightScript : public vrm::ScriptComponent
{
public:
    MovingLightScript(float circleRadius, float height, float startAngle, float speed);
    ~MovingLightScript() = default;

    void onUpdate(float dt) override;

private:
    float m_CircleRadius;
    float m_Height;
    float m_Angle;
    float m_Speed;
};