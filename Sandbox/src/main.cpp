#include <iostream>
#include <array>
#include <cmath>

#include <Vroom/Core/Application.h>
#include <Vroom/Scene/Scene.h>
#include <Vroom/Asset/Asset.h>
#include <Vroom/Render/Renderer.h>
 
#include <Vroom/Scene/Components/MeshComponent.h>
#include <Vroom/Scene/Components/TransformComponent.h>
#include <Vroom/Scene/Components/PointLightComponent.h>
#include <glm/gtx/rotate_vector.hpp>

#include "Scripts/SuzanneScript.h"

class MyScene : public vrm::Scene
{
public:
	MyScene(const std::string& littleNickname) : vrm::Scene(), m_LittleNickName(littleNickname) {}
	~MyScene() = default;
 
protected:
	void onInit() override
	{
		// Set a custom camera
		setCamera(&myCamera);
		myCamera.setWorldPosition({0.f, 0.f, 5.f});

		// Some lights in a circle of radius 10 in xz, and y = 10 and various colors (always positive colors)
		for (uint8_t i = 0; i < 10; i++)
		{
			auto entity = createEntity("PointLight_" + std::to_string(i));
			entity.addComponent<vrm::PointLightComponent>(glm::vec3{ std::cos(i * 2.f * 3.14159f / 10), 1.f, std::sin(i * 2.f * 3.14159f / 10) }, 50.f, 15.f);
			auto mesh = getAssetManager().getAsset<vrm::MeshAsset>("Resources/Meshes/sphere.obj");
			entity.addComponent<vrm::MeshComponent>(mesh);
			auto& transform = entity.getComponent<vrm::TransformComponent>();
			float theta = i * 2.f * 3.14159f / 10;
			transform.setPosition({10.f * std::cos(theta), 10.f, 10.f * std::sin(theta)});
			transform.setScale({0.2f, 0.2f, 0.2f});
		}

		// Place a cube in the center of the scene
		{
			auto entity = createEntity("ColoredCube");
			auto mesh = getAssetManager().getAsset<vrm::MeshAsset>("Resources/Meshes/Textured_Cube.obj");
			entity.addComponent<vrm::MeshComponent>(mesh);
			auto& transform = entity.getComponent<vrm::TransformComponent>();
			transform.setPosition({0.f, 0.f, 0.f});
		}

		// Place a plane under the cube
		{
			auto entity = createEntity("Plane");
			auto mesh = getAssetManager().getAsset<vrm::MeshAsset>("Resources/Meshes/Textured_Cube.obj");
			entity.addComponent<vrm::MeshComponent>(mesh);
			auto& transform = entity.getComponent<vrm::TransformComponent>();
			transform.setPosition({0.f, -1.f, 0.f});
			transform.setScale({50.f, 0.1f, 50.f});
		}
 
		// Create a few suzannes
		for (uint8_t i = 0; i < 10; i++)
		{
			auto entity = createEntity("Suzanne_" + std::to_string(i));
			auto mesh = getAssetManager().getAsset<vrm::MeshAsset>("Resources/Meshes/Suzanne.obj");
			entity.addComponent<vrm::MeshComponent>(mesh);
			entity.addScriptComponent<SuzanneScript>(suzanneRadius, i * glm::two_pi<float>() / 10, suzanneSpeed);
		}

		// Bind triggers to the camera
		// This is a bit ugly. I might create some facilities that do this job in the future.
		// Maybe another event type, which will give a scalar depending on the input (moveForward in [-1, 1] for example, controlled with any input we want).
		getApplication().getTrigger("MoveForward")
			.bindCallback([this](bool triggered) { forwardValue += triggered ? 1.f : -1.f; });
		getApplication().getTrigger("MoveBackward")
			.bindCallback([this](bool triggered) { forwardValue -= triggered ? 1.f : -1.f; });
		getApplication().getTrigger("MoveRight")
			.bindCallback([this](bool triggered) { rightValue += triggered ? 1.f : -1.f; });
		getApplication().getTrigger("MoveLeft")
			.bindCallback([this](bool triggered) { rightValue -= triggered ? 1.f : -1.f; });
		getApplication().getTrigger("MoveUp")
			.bindCallback([this](bool triggered) { upValue += triggered ? 1.f : -1.f; });
		getApplication().getTrigger("MoveDown")
			.bindCallback([this](bool triggered) { upValue -= triggered ? 1.f : -1.f; });
		
		getApplication().getCustomEvent("MouseMoved")
			.bindCallback([this](const vrm::Event& event) {
				turnRightValue += static_cast<float>(event.mouseDeltaX);
				lookUpValue -= static_cast<float>(event.mouseDeltaY);
			});

		LOG_TRACE("MyScene \"{}\" instance initialized.", m_LittleNickName);
	}

	void onUpdate(float dt) override
	{
		myCamera.move(forwardValue * myCameraSpeed * dt * myCamera.getForwardVector());
		myCamera.move(rightValue * myCameraSpeed * dt * myCamera.getRightVector());
		myCamera.move(upValue * myCameraSpeed * dt * glm::vec3{0.f, 1.f, 0.f});
		myCamera.addYaw(turnRightValue * myCameraAngularSpeed);
		myCamera.addPitch(lookUpValue * myCameraAngularSpeed);

		lookUpValue = 0.f;
		turnRightValue = 0.f;

		m_TimeAccumulator += dt;
		m_FramesAccumulator++;

		if (m_TimeAccumulator < 1.f) return;

		LOG_TRACE("fps: {}", m_FramesAccumulator);

		m_FramesAccumulator = 0;
		m_TimeAccumulator -= 1.f;
	}

private:
	std::string m_LittleNickName;

	// Low cost profiler
	float m_TimeAccumulator = 0.f;
	size_t m_FramesAccumulator = 0;

	// This should be stored in a CameraComponent.
	vrm::FirstPersonCamera myCamera{0.1f, 100.f, glm::radians(90.f), 600.f / 400.f, glm::vec3{0.f, 0.f, 0.f}, glm::vec3{0.f, 0.f, 0.f}};

	float forwardValue = 0.f, rightValue = 0.f, upValue = 0.f;
	float turnRightValue = 0.f, lookUpValue = 0.f;
	float myCameraSpeed = 10.f, myCameraAngularSpeed = .08f * glm::two_pi<float>() / 360.f;
 
	float suzanneRadius = 10.f;
	float suzanneSpeed = 3.14159f / 4.f;
};

int main(int argc, char** argv)
{
	// Create the application
	vrm::Application app{argc, argv};
	
	// Create some triggers so we can interact with the application
	app.createTrigger("MoveForward")
		.bindInput(vrm::KeyCode::W);
	app.createTrigger("MoveBackward")
		.bindInput(vrm::KeyCode::S);
	app.createTrigger("MoveLeft")
		.bindInput(vrm::KeyCode::A);
	app.createTrigger("MoveRight")
		.bindInput(vrm::KeyCode::D);
	app.createTrigger("MoveUp")
		.bindInput(vrm::KeyCode::Space);
	app.createTrigger("MoveDown")
		.bindInput(vrm::KeyCode::LeftShift);

	// Create some custom events, which are more general than triggers
	app.createCustomEvent("Exit")
		.bindInput(vrm::Event::Type::KeyPressed, vrm::KeyCode::Escape)
		.bindInput(vrm::Event::Type::Exit)
		.bindCallback([&app](const vrm::Event&) { LOG_INFO("Application exit has been requested by user."); app.exit(); });
	app.createCustomEvent("MouseMoved")
		.bindInput(vrm::Event::Type::MouseMoved);
	app.createCustomEvent("ReleaseMouse")
		.bindInput(vrm::Event::Type::KeyPressed, vrm::KeyCode::LeftAlt)
		.bindCallback([&app](const vrm::Event&) { app.getWindow().setCursorVisible(true); });
	app.createCustomEvent("MouseEnter")
		.bindInput(vrm::Event::Type::MouseEntered)
		.bindCallback([&app](const vrm::Event&) { app.getWindow().setCursorVisible(false); });
	
	// Load the custom scene, defined above
	app.loadScene<MyScene>("A cute little scene !");

	// Run the application
	app.run();

	return 0;
}
