#include <iostream>

#include <Vroom/Core/Application.h>
#include <Vroom/Scene/Scene.h>
#include <Vroom/Asset/Asset.h>
#include <Vroom/Render/Renderer.h>

class MyScene : public vrm::Scene
{
public:
	MyScene(const std::string& littleNickname) : vrm::Scene(), m_LittleNickName(littleNickname) {}
	~MyScene() = default;

protected:
	void onInit() override
	{
		myMesh = getAssetManager().getAsset<vrm::MeshAsset>("Resources/Meshes/cube.obj");
		myShader.loadFromFile("Resources/Shaders/vert_Basic.glsl", "Resources/Shaders/frag_Basic.glsl");
		setCamera(&myCamera);

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

		m_TimeAccumulator += dt;
		m_FramesAccumulator++;

		if (m_TimeAccumulator < 1.f) return;

		LOG_TRACE("fps: {}", m_FramesAccumulator);

		m_FramesAccumulator = 0;
		m_TimeAccumulator -= 1.f;
	}

	void onRender() override
	{
		getApplication().getRenderer().drawMesh(myMesh.getStaticAsset()->getRenderMesh(), myShader, getCamera(), glm::mat4(1.f));
	}

private:
	std::string m_LittleNickName;
	float m_TimeAccumulator = 0.f;
	size_t m_FramesAccumulator = 0;

	vrm::FirstPersonCamera myCamera{0.1f, 100.f, 45.f, 600.f / 400.f, glm::vec3{0.f, 0.f, 0.f}, glm::vec3{0.f, 0.f, 0.f}};
	vrm::MeshInstance myMesh;
	Shader myShader;

	float forwardValue = 0.f, rightValue = 0.f, upValue = 0.f;
	float turnRightValue = 0.f, lookUpValue = 0.f;
	float myCameraSpeed = 10.f, myCameraAngularSpeed = 100.f;
};

int main(int argc, char** argv)
{
	vrm::Application app{argc, argv};
	
	app.createTrigger("MoveForward")
		.bindInput(vrm::KeyCode::W)
		.bindCallback([](bool triggered) { LOG_INFO("Moving forward  : {}", triggered); });
	app.createTrigger("MoveBackward")
		.bindInput(vrm::KeyCode::S)
		.bindCallback([](bool triggered) { LOG_INFO("Moving backward : {}", triggered); });
	app.createTrigger("MoveLeft")
		.bindInput(vrm::KeyCode::A)
		.bindCallback([](bool triggered) { LOG_INFO("Moving left     : {}", triggered); });
	app.createTrigger("MoveRight")
		.bindInput(vrm::KeyCode::D)
		.bindCallback([](bool triggered) { LOG_INFO("Moving right    : {}", triggered); });
	app.createTrigger("MoveUp")
		.bindInput(vrm::KeyCode::Space)
		.bindCallback([](bool triggered) { LOG_INFO("Moving up       : {}", triggered); });
	app.createTrigger("MoveDown")
		.bindInput(vrm::KeyCode::LeftShift)
		.bindCallback([](bool triggered) { LOG_INFO("Moving down     : {}", triggered); });
	app.createTrigger("TurnLeft")
		.bindInput(vrm::KeyCode::Left)
		.bindCallback([](bool triggered) { LOG_INFO("Turning left    : {}", triggered); });
	app.createTrigger("TurnRight")
		.bindInput(vrm::KeyCode::Right)
		.bindCallback([](bool triggered) { LOG_INFO("Turning right   : {}", triggered); });
	app.createTrigger("LookUp")
		.bindInput(vrm::KeyCode::Up)
		.bindCallback([](bool triggered) { LOG_INFO("Looking up      : {}", triggered); });
	app.createTrigger("LookDown")
		.bindInput(vrm::KeyCode::Down)
		.bindCallback([](bool triggered) { LOG_INFO("Looking down    : {}", triggered); });

	app.createCustomEvent("Exit")
		.bindInput(vrm::Event::Type::KeyPressed, vrm::KeyCode::Escape)
		.bindInput(vrm::Event::Type::Exit)
		.bindCallback([&app](const vrm::Event&) { LOG_INFO("Exit custom event."); app.exit(); });
	
	app.loadScene<MyScene>("A cute little scene !");

	app.run();

	return 0;
}
