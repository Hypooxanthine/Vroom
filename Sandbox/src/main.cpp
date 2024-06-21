#include <iostream>
#include <array>
#include <cmath>

#include <Vroom/Core/Application.h>
#include <Vroom/Scene/Scene.h>
#include <Vroom/Asset/Asset.h>
#include <Vroom/Render/Renderer.h>

#include <Vroom/Scene/Components/MeshComponent.h>
#include <Vroom/Scene/Components/TransformComponent.h>
#include <glm/gtx/rotate_vector.hpp>

class MyScene : public vrm::Scene
{
public:
	MyScene(const std::string& littleNickname) : vrm::Scene(), m_LittleNickName(littleNickname) {}
	~MyScene() = default;

protected:
	void onInit() override
	{
		// Create a few suzannes
		for (uint8_t i = 0; i < suzannes.size(); i++)
		{
			auto entity = createEntity("Suzanne_" + std::to_string(i));
			auto mesh = getAssetManager().getAsset<vrm::MeshAsset>("Resources/Meshes/Suzanne.obj");
			entity.addComponent<vrm::MeshComponent>(mesh);
			auto& transform = entity.getComponent<vrm::TransformComponent>();

			float theta = 3.14159f * 2.f * i / suzannes.size();
			transform.position = { std::cos(theta) * suzanneRadius, 0.f, -std::sin(theta) * suzanneRadius };
			transform.rotation = { 0.f, glm::degrees(theta), 0.f };

			suzannes[i] = entity;
		}

		// Set a custom camera
		setCamera(&myCamera);

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
		getApplication().getTrigger("TurnRight")
			.bindCallback([this](bool triggered) { turnRightValue += triggered ? 1.f : -1.f; });
		getApplication().getTrigger("TurnLeft")
			.bindCallback([this](bool triggered) { turnRightValue -= triggered ? 1.f : -1.f; });
		getApplication().getTrigger("LookUp")
			.bindCallback([this](bool triggered) { lookUpValue += triggered ? 1.f : -1.f; });
		getApplication().getTrigger("LookDown")
			.bindCallback([this](bool triggered) { lookUpValue -= triggered ? 1.f : -1.f; });

		LOG_TRACE("MyScene \"{}\" instance initialized.", m_LittleNickName);
	}

	void onUpdate(float dt) override
	{
		myCamera.move(forwardValue * myCameraSpeed * dt * myCamera.getForwardVector());
		myCamera.move(rightValue * myCameraSpeed * dt * myCamera.getRightVector());
		myCamera.move(upValue * myCameraSpeed * dt * glm::vec3{0.f, 1.f, 0.f});
		myCamera.addYaw(turnRightValue * myCameraAngularSpeed * dt);
		myCamera.addPitch(lookUpValue * myCameraAngularSpeed * dt);

		// Rotate suzannes on their circle
		for (auto& suzanne : suzannes)
		{
			auto& transform = suzanne.getComponent<vrm::TransformComponent>();
			float theta = -suzanneSpeed * dt;
			transform.position = glm::rotateY(transform.position, theta);
			transform.rotation.y += glm::degrees(theta);
		}

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
	vrm::FirstPersonCamera myCamera{0.1f, 100.f, 45.f, 600.f / 400.f, glm::vec3{0.f, 0.f, 0.f}, glm::vec3{0.f, 0.f, 0.f}};

	float forwardValue = 0.f, rightValue = 0.f, upValue = 0.f;
	float turnRightValue = 0.f, lookUpValue = 0.f;
	float myCameraSpeed = 10.f, myCameraAngularSpeed = 100.f;
 
	std::array<vrm::Entity, 10> suzannes;
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
	app.createTrigger("TurnLeft")
		.bindInput(vrm::KeyCode::Left);
	app.createTrigger("TurnRight")
		.bindInput(vrm::KeyCode::Right);
	app.createTrigger("LookUp")
		.bindInput(vrm::KeyCode::Up);
	app.createTrigger("LookDown")
		.bindInput(vrm::KeyCode::Down);

	// Create some custom events, which are more general than triggers
	app.createCustomEvent("Exit")
		.bindInput(vrm::Event::Type::KeyPressed, vrm::KeyCode::Escape)
		.bindInput(vrm::Event::Type::Exit)
		.bindCallback([&app](const vrm::Event&) { LOG_INFO("Application exit has been requested by user."); app.exit(); });
	
	// Load the custom scene, defined above
	app.loadScene<MyScene>("A cute little scene !");

	// Run the application
	app.run();

	return 0;
}
