#include "VroomEditor/TestScene/TestScene.h"

#include <Vroom/Core/Application.h>
#include <Vroom/Core/Window.h>
#include <Vroom/Core/GameLayer.h>
#include <Vroom/Scene/Scene.h>
#include <Vroom/Asset/Asset.h>

#include <Vroom/Scene/Components/MeshComponent.h>
#include <Vroom/Scene/Components/TransformComponent.h>
#include <Vroom/Scene/Components/PointLightComponent.h>

#include "VroomEditor/TestScene/SuzanneScript.h"
#include "VroomEditor/TestScene/MovingLightScript.h"


namespace vrm
{


void TestScene::onInit()
{
    auto& gameLayer = vrm::Application::Get().getGameLayer();

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
        
	gameLayer.createCustomEvent("MouseMoved")
		.bindInput(vrm::Event::Type::MouseMoved);
	gameLayer.createCustomEvent("ReleaseMouse")
		.bindInput(vrm::Event::Type::KeyPressed, vrm::KeyCode::LeftAlt)
		.bindCallback([](const vrm::Event&) { vrm::Application::Get().getWindow().setCursorVisible(true); });
	gameLayer.createCustomEvent("MouseEnter")
		.bindInput(vrm::Event::Type::MouseEntered)
		.bindCallback([](const vrm::Event&) { vrm::Application::Get().getWindow().setCursorVisible(false); });

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
}

void TestScene::onUpdate(float dt)
{
    myCamera.move(forwardValue * myCameraSpeed * dt * myCamera.getForwardVector());
    myCamera.move(rightValue * myCameraSpeed * dt * myCamera.getRightVector());
    myCamera.move(upValue * myCameraSpeed * dt * glm::vec3{0.f, 1.f, 0.f});
    myCamera.addYaw(turnRightValue * myCameraAngularSpeed);
    myCamera.addPitch(lookUpValue * myCameraAngularSpeed);

    lookUpValue = 0.f;
    turnRightValue = 0.f;
}

void TestScene::onRender()
{
}

void TestScene::onEnd()
{
}

} // namespace vrm