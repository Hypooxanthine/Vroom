#include <Vroom/Core/Application.h>

#include <Vroom/Scene/Scene.h>
#include <Vroom/Scene/Entity.h>

class MyScript : public Vroom::ScriptComponent
{
public:
	MyScript()
	{
		Vroom::Application::Get().registerEventCallback("Exit", VR_BIND_FN(onExit));
	}

	virtual void onUpdate() override
	{
		if (getEventState("MoveLeft"))
			moveVector += { -1.f, 0.f};
		if (getEventState("MoveRight"))
			moveVector += { 1.f, 0.f};
		if (getEventState("MoveUp"))
			moveVector += { 0.f, -1.f };
		if (getEventState("MoveDown"))
			moveVector += { 0.f, 1.f };

		float moveLength = std::sqrt(moveVector.x * moveVector.x + moveVector.y * moveVector.y);

		if (moveLength > 0.0001f)
		{
			moveVector = moveVector * getDeltaTime() * moveSpeed / moveLength;
		}

		auto& transform = getEntity().getComponent<Vroom::TransformComponent>();
		transform.setTranslation(transform.getTranslation() + moveVector);

		moveVector = { 0.f, 0.f };
	}

	void onExit(bool pressed)
	{
		quitGame();
	}

private:
	float moveSpeed = 200.f;
	sf::Vector2f moveVector;
};

class MyScene : public Vroom::Scene
{
public:
	MyScene()
	{
		// Player
		auto player = createEntity();
		auto& spriteComponent = player.addComponent<Vroom::SpriteComponent>
			(getAssetManager().getAsset<Vroom::Sprite>("Data/Sprites/SpriteTest.sprite"));
		spriteComponent.setTranslation(-590.f / 2.f * .2f, -576.f / 2.f * .2f);

		player.addScriptComponent<MyScript>();

		auto& transformComponent = player.getComponent<Vroom::TransformComponent>();
		transformComponent.setScale(.2f);

		auto& cameraComponent = player.addComponent<Vroom::CameraComponent>();
		cameraComponent.setTranslation(64.f * 10.f / 2.f, 64.f * 10.f / 2.f);
		cameraComponent.setSize((sf::Vector2f)Vroom::Application::Get().getWindowSize() * 0.79771f);
		cameraComponent.activate(); // An "auto activation" option can be set in the future engine editor. It'll generate this line.

		// Tiles
		for (size_t i = 0; i < 10; i++)
		{
			for (size_t j = 0; j < 10; j++)
			{
				auto topBrick = createEntity();
				topBrick.addComponent<Vroom::SpriteComponent>(getAssetManager().getAsset<Vroom::Sprite>("Data/Sprites/brick.sprite"));
				topBrick.getComponent<Vroom::TransformComponent>().setTranslation(64.f * i, 64.f * j);
			}
		}
	}
};

int main()
{
	std::unique_ptr<Vroom::Application> app = std::make_unique<Vroom::Application>(64 * 10, 64 * 10, "A Vroom game");

	app->registerEvent("MoveLeft");
	app->registerEvent("MoveRight");
	app->registerEvent("MoveUp");
	app->registerEvent("MoveDown");
	app->registerEvent("Exit");

	app->bindEventInput("MoveLeft", sf::Keyboard::Q);
	app->bindEventInput("MoveLeft", sf::Keyboard::Left);
	app->bindEventInput("MoveRight", sf::Keyboard::D);
	app->bindEventInput("MoveRight", sf::Keyboard::Right);
	app->bindEventInput("MoveUp", sf::Keyboard::Z);
	app->bindEventInput("MoveUp", sf::Keyboard::Up);
	app->bindEventInput("MoveDown", sf::Keyboard::S);
	app->bindEventInput("MoveDown", sf::Keyboard::Down);
	app->bindEventInput("Exit", sf::Keyboard::Escape);

	app->setScene(new MyScene());

	app->run();

	return 0;
}