#pragma once

#include <Vroom/Scene/Scene.h>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace vrm
{

class TestScene : public vrm::Scene
{
public:
    TestScene() = default;
    ~TestScene() = default;

protected:
    void onInit() override;
    void onUpdate(float dt) override;
    void onRender() override;
    void onEnd() override;

private:
	// This should be stored in a CameraComponent.
	vrm::FirstPersonCamera myCamera{0.1f, 100.f, glm::radians(90.f), 600.f / 400.f, glm::vec3{0.f, 0.f, 0.f}, glm::vec3{0.f, 0.f, 0.f}};

	float forwardValue = 0.f, rightValue = 0.f, upValue = 0.f;
	float turnRightValue = 0.f, lookUpValue = 0.f;
	float myCameraSpeed = 10.f, myCameraAngularSpeed = .08f * glm::two_pi<float>() / 360.f;
 
	float suzanneRadius = 10.f;
	float suzanneSpeed = 3.14159f / 4.f;

	float lightRadius = 10.f;
	float lightSpeed = -3.14159f / 4.f;
};

} // namespace vrm