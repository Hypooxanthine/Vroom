#pragma once

#include <Vroom/Scene/Components/ScriptComponent.h>

class MovingLightScript : public vrm::ScriptComponent
{
public:
    MovingLightScript();
    ~MovingLightScript();

    void init(float circleRadius, float height, float startAngle, float speed);

    void onSpawn() override;
    void onUpdate(const vrm::DeltaTime& dt) override;
    void onDestroy() override;

private:
    float m_CircleRadius;
    float m_Height;
    float m_Angle;
    float m_Speed;
};