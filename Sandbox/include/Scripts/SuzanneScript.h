#pragma once

#include <Vroom/Scene/Components/ScriptComponent.h>

class SuzanneScript : public vrm::ScriptComponent
{
public:
    SuzanneScript(float circleRadius, float startAngle, float speed);
    ~SuzanneScript();

    void onSpawn() override;
    void onUpdate(const vrm::DeltaTime& dt) override;
    void onDestroy() override;

private:
    float m_CircleRadius;
    float m_Angle;
    float m_Speed;
};