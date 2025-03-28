#include <iostream>
#include <array>
#include <cmath>
#include <fstream>

#include <Vroom/Core/Application.h>
#include <Vroom/Core/Window.h>
#include <Vroom/Core/GameLayer.h>
#include <Vroom/Scene/Scene.h>
#include <Vroom/Asset/Asset.h>
#include <Vroom/Render/Renderer.h>
 
#include <Vroom/Scene/Components/MeshComponent.h>
#include <Vroom/Scene/Components/TransformComponent.h>
#include <Vroom/Scene/Components/PointLightComponent.h>

#include <Vroom/Asset/Parsing/ShaderParsing.h>
#include <Vroom/Asset/StaticAsset/ShaderExecAsset.h>
#include <Vroom/Asset/AssetData/ShaderData.h>

#include <glm/gtx/rotate_vector.hpp>

#include "Scripts/SuzanneScript.h"
#include "Scripts/MovingLightScript.h"

class MyScene : public vrm::Scene
{
public:
	MyScene(const std::string& littleNickname) : vrm::Scene(), m_LittleNickName(littleNickname) {}
	~MyScene() = default;
 
protected:
	void onInit() override
	{
    // Test shader parsing
    // using SD = vrm::ShaderData;
    // SD sd;
    // sd.setShaderEnabled(SD::EShaderType::eVertex, true);
    // sd.setShaderEnabled(SD::EShaderType::eFragment, true);
    // sd.setVersion(SD::EShaderType::eAll, "450");
    // sd.addDefine(SD::EShaderType::eVertex, { "MY_DEFINE", "1" });
    // sd.addDefine(SD::EShaderType::eFragment, { "MY_DEFINE2" });
    // sd.addExtension(SD::EShaderType::eVertex, { "GL_ARB_multi_draw_indirect", SD::Extension::EBehaviour::eEnable });
    // sd.addExtension(SD::EShaderType::eAll, { "GL_ARB_shader_bit_encoding", SD::Extension::EBehaviour::eRequire });
    // sd.addVarying(SD::EShaderType::eVertex, { "vec3", "myVarying" });
    // sd.addVarying(SD::EShaderType::eFragment, { "vec3", "myVarying" });
    // sd.addVarying(SD::EShaderType::eFragment, { "vec3", "myVarying2", SD::Varying::EQualifier::eFlat });
    // sd.addUniform(SD::EShaderType::eVertex, { "vec3", "myUniform" });
    // sd.addUniform(SD::EShaderType::eFragment, { "vec3", "myUniform" });
    // sd.addUniformBuffer(SD::EShaderType::eVertex, { "std140", "myUniformBuffer", {
    //   { "myUniform", "vec3" },
    //   { "myUniform2", "uint" }
    // } });
    // sd.addUniformBuffer(SD::EShaderType::eFragment, { "std140", "myUniformBuffer", {
    //   { "myUniform", "float" },
    //   { "myUniform2[]", "int" }
    // } });
    // sd.addStorageBuffer(
    //   SD::EShaderType::eVertex,
    //   {
    //     "std430",
    //     { SD::EMemoryQualifier::eCoherent, SD::EMemoryQualifier::eReadOnly },
    //     "myStorageBuffer",
    //     {
    //       { "myStorageBuffer", "vec3", "", {SD::EMemoryQualifier::eRestrict} },
    //       { "myStorageBuffer2", "uint", "SOME_DEFINE" }
    //     },
    //     "SOME_DEFINE2"
    //   }
    // );
    // sd.addEarlySource(
    //   SD::EShaderType::eVertex,
    //   { 
    //     "void main() { myVarying = myUniform; }",
    //     "myFile.glsl",
    //     "MY_DEFINE"
    //   }
    // );
    // sd.addSource(
    //   SD::EShaderType::eFragment,
    //   { 
    //     "void main() { myVarying = myUniform; }",
    //     "myFile2.glsl",
    //     "MY_DEFINE"
    //   }
    // );

    const auto& sd = vrm::AssetManager::Get().getAsset<vrm::ShaderExecAsset>("Resources/Shaders/Test_ShaderData.json").getStaticAsset()->getShaderData();

    auto fullShader = sd.combine();
    std::ofstream ofs("compute.glsl");
    ofs << fullShader.compute.sourceCode;
    ofs.close();
    ofs.open("vertex.glsl");
    ofs << fullShader.vertex.sourceCode;
    ofs.close();
    ofs.open("geometry.glsl");
    ofs << fullShader.geometry.sourceCode;
    ofs.close();
    ofs.open("fragment.glsl");
    ofs << fullShader.fragment.sourceCode;
    ofs.close();

    vrm::Application::Get().exit();

		// Set a custom camera
		setCamera(&myCamera);
		myCamera.setWorldPosition({0.f, 0.f, 5.f});

		// Some lights in a circle of radius 10 in xz, and y = 10 and various colors (always positive colors)
		for (uint8_t i = 0; i < 10; i++)
		{
			auto entity = createEntity("PointLight_" + std::to_string(i));
			entity.addComponent<vrm::PointLightComponent>(glm::vec3{ std::cos(i * 2.f * 3.14159f / 10), 1.f, std::sin(i * 2.f * 3.14159f / 10) }, 50.f, 15.f);
			auto mesh = vrm::AssetManager::Get().getAsset<vrm::MeshAsset>("Resources/Meshes/sphere.obj");
			entity.addComponent<vrm::MeshComponent>(mesh);
			entity.addScriptComponent<MovingLightScript>(lightRadius, 10.f, i * glm::two_pi<float>() / 10, lightSpeed);
			auto& transform = entity.getComponent<vrm::TransformComponent>();
			transform.setScale({0.2f, 0.2f, 0.2f});
		}

		// Place a cube in the center of the scene
		{
			auto entity = createEntity("ColoredCube");
			auto mesh = vrm::AssetManager::Get().getAsset<vrm::MeshAsset>("Resources/Meshes/Textured_Cube.obj");
			entity.addComponent<vrm::MeshComponent>(mesh);
			auto& transform = entity.getComponent<vrm::TransformComponent>();
			transform.setPosition({0.f, 0.f, 0.f});
		}

		// Place a plane under the cube
		{
			auto entity = createEntity("Plane");
			auto mesh = vrm::AssetManager::Get().getAsset<vrm::MeshAsset>("Resources/Meshes/Textured_Cube.obj");
			entity.addComponent<vrm::MeshComponent>(mesh);
			auto& transform = entity.getComponent<vrm::TransformComponent>();
			transform.setPosition({0.f, -1.f, 0.f});
			transform.setScale({50.f, 0.1f, 50.f});
		}
 
		// Create a few suzannes
		for (uint8_t i = 0; i < 10; i++)
		{
			auto entity = createEntity("Suzanne_" + std::to_string(i));
			auto mesh = vrm::AssetManager::Get().getAsset<vrm::MeshAsset>("Resources/Meshes/Suzanne.obj");
			entity.addComponent<vrm::MeshComponent>(mesh);
			entity.addScriptComponent<SuzanneScript>(suzanneRadius, i * glm::two_pi<float>() / 10, suzanneSpeed);
		}

		auto& gameLayer = vrm::Application::Get().getGameLayer();

		// Bind triggers to the camera
		// This is a bit ugly. I might create some facilities that do this job in the future.
		// Maybe another event type, which will give a scalar depending on the input (moveForward in [-1, 1] for example, controlled with any input we want).
		gameLayer.getTrigger("MoveForward")
			.bindCallback([this](bool triggered) { forwardValue += triggered ? 1.f : -1.f; });
		gameLayer.getTrigger("MoveBackward")
			.bindCallback([this](bool triggered) { forwardValue -= triggered ? 1.f : -1.f; });
		gameLayer.getTrigger("MoveRight")
			.bindCallback([this](bool triggered) { rightValue += triggered ? 1.f : -1.f; });
		gameLayer.getTrigger("MoveLeft")
			.bindCallback([this](bool triggered) { rightValue -= triggered ? 1.f : -1.f; });
		gameLayer.getTrigger("MoveUp")
			.bindCallback([this](bool triggered) { upValue += triggered ? 1.f : -1.f; });
		gameLayer.getTrigger("MoveDown")
			.bindCallback([this](bool triggered) { upValue -= triggered ? 1.f : -1.f; });
		
		gameLayer.getCustomEvent("MouseMoved")
			.bindCallback([this](const vrm::Event& event) {
				turnRightValue += static_cast<float>(event.mouseDeltaX);
				lookUpValue -= static_cast<float>(event.mouseDeltaY);
			});

		VRM_LOG_TRACE("MyScene \"{}\" instance initialized.", m_LittleNickName);
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

		VRM_LOG_TRACE("fps: {}", m_FramesAccumulator);

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

	float lightRadius = 10.f;
	float lightSpeed = -3.14159f / 4.f;
};

int main(int argc, char** argv)
{
	// Create the application
	vrm::Application app{argc, argv};

	auto& gameLayer = app.getGameLayer();
	
	// Create some triggers so we can interact with the application
	gameLayer.createTrigger("MoveForward")
		.bindInput(vrm::KeyCode::W);
	gameLayer.createTrigger("MoveBackward")
		.bindInput(vrm::KeyCode::S);
	gameLayer.createTrigger("MoveLeft")
		.bindInput(vrm::KeyCode::A);
	gameLayer.createTrigger("MoveRight")
		.bindInput(vrm::KeyCode::D);
	gameLayer.createTrigger("MoveUp")
		.bindInput(vrm::KeyCode::Space);
	gameLayer.createTrigger("MoveDown")
		.bindInput(vrm::KeyCode::LeftShift);

	// Create some custom events, which are more general than triggers
	gameLayer.createCustomEvent("Exit")
		.bindInput(vrm::Event::Type::KeyPressed, vrm::KeyCode::Escape)
		.bindInput(vrm::Event::Type::Exit)
		.bindCallback([&app](const vrm::Event&) { VRM_LOG_INFO("Application exit has been requested by user."); app.exit(); });
	gameLayer.createCustomEvent("MouseMoved")
		.bindInput(vrm::Event::Type::MouseMoved);
	gameLayer.createCustomEvent("ReleaseMouse")
		.bindInput(vrm::Event::Type::KeyPressed, vrm::KeyCode::LeftAlt)
		.bindCallback([&app](const vrm::Event&) { app.getWindow().setCursorVisible(true); });
	gameLayer.createCustomEvent("MouseEnter")
		.bindInput(vrm::Event::Type::MouseEntered)
		.bindCallback([&app](const vrm::Event&) { app.getWindow().setCursorVisible(false); });
	
	// Load the custom scene, defined above
	gameLayer.loadScene<MyScene>("A cute little scene !");

	// Run the application
	app.run();

	return 0;
}
