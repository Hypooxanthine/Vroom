#include <Vroom/Core/Application.h>

#include <Vroom/Scene/Scene.h>
#include <Vroom/Scene/Entity.h>
#include <Vroom/Core/Application.h>

class MyScript : public Vroom::ScriptComponent
{
public:
	MyScript()
	{
		Vroom::Application::Get().registerEventCallback("Exit", VR_BIND_FN(onExit));
		Vroom::Application::Get().registerEventCallback("SpeedBoost", VR_BIND_FN(onBoost));
	}

	void onBoost(bool pressed)
	{
		if (pressed)
			currentSpeed = boostSpeed;
		else
			currentSpeed = normalSpeed;
	}

	void onExit(bool pressed)
	{
		quitGame();
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
			moveVector = moveVector * getDeltaTime() * currentSpeed / moveLength;
		}

		auto& transform = getEntity().getComponent<Vroom::TransformComponent>();
		transform.setTranslation(transform.getTranslation() + moveVector);

		moveVector = { 0.f, 0.f };
	}

private:
	float normalSpeed = 200.f;
	float boostSpeed = 4000.f;
	float currentSpeed = normalSpeed;
	sf::Vector2f moveVector;
};

class MyScene : public Vroom::Scene
{
public:
	MyScene()
		: Scene({-1500.f, -1500.f, 3000.f, 3000.f })
	{
		// For debugging
		if (IS_DEBUG())
		{
			setShowCamera(true);
			setShowChunks(true);
		}

		// Player
		auto player = createEntity("Player");
		auto& spriteComponent = player.addComponent<Vroom::SpriteComponent>
			(getAssetManager().getAsset<Vroom::Sprite>("Data/Sprites/SpriteTest.sprite"));
		spriteComponent.setTranslation(-590.f / 2.f * .2f, -576.f / 2.f * .2f);

		player.addScriptComponent<MyScript>();
		player.addComponent<Vroom::LayerComponent>(10);

		auto& transformComponent = player.getComponent<Vroom::TransformComponent>();
		transformComponent.setScale(.2f);

		auto& cameraComponent = player.addComponent<Vroom::CameraComponent>();
		cameraComponent.setTranslation(64.f * 10.f / 2.f, 64.f * 10.f / 2.f);
		cameraComponent.setSize((sf::Vector2f)Vroom::Application::Get().getWindowSize() * 0.79771f);
		cameraComponent.activate(); // An "auto activation" option can be set in the future engine editor. It'll generate this line.

		// Tiles
		for (int i = -20; i < 20; i++)
		{
			for (int j = -20; j < 20; j++)
			{
				auto brick = createEntity();
				brick.addComponent<Vroom::SpriteComponent>(getAssetManager().getAsset<Vroom::Sprite>("Data/Sprites/brick.sprite"));
				brick.getComponent<Vroom::TransformComponent>().setTranslation(64.f * i, 64.f * j);
				brick.addComponent<Vroom::LayerComponent>(0);
			}
		}
	}

protected:
	virtual void onRender() override
	{
		frames++;
		ellapsedTime += Vroom::Application::Get().getDeltaTime();

		if (ellapsedTime > 1.f)
		{
			std::cout << "FPS: " << (float)frames / ellapsedTime << "." << std::endl;;
			ellapsedTime -= 1.f;
			frames = 0;
		}
	}

private:
	size_t frames = 0;
	float ellapsedTime = 0.f;
};

int main()
{
	{
		std::unique_ptr<Vroom::Application> app = std::make_unique<Vroom::Application>(64 * 10, 64 * 10, "A Vroom game");

		app->registerEvent("MoveLeft");
		app->registerEvent("MoveRight");
		app->registerEvent("MoveUp");
		app->registerEvent("MoveDown");
		app->registerEvent("SpeedBoost");
		app->registerEvent("Exit");

		app->bindEventInput("MoveLeft", sf::Keyboard::Q);
		app->bindEventInput("MoveLeft", sf::Keyboard::Left);
		app->bindEventInput("MoveRight", sf::Keyboard::D);
		app->bindEventInput("MoveRight", sf::Keyboard::Right);
		app->bindEventInput("MoveUp", sf::Keyboard::Z);
		app->bindEventInput("MoveUp", sf::Keyboard::Up);
		app->bindEventInput("MoveDown", sf::Keyboard::S);
		app->bindEventInput("MoveDown", sf::Keyboard::Down);
		app->bindEventInput("SpeedBoost", sf::Keyboard::LShift);
		app->bindEventInput("SpeedBoost", sf::Keyboard::Space);
		app->bindEventInput("Exit", sf::Keyboard::Escape);

		app->setScene(new MyScene());

		app->run();
	}

	std::cin.get();

	return 0;
}