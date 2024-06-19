#include <iostream>

#include <Vroom/Core/Application.h>
#include <Vroom/Scene/Scene.h>

class MyScene : public vrm::Scene
{
public:
	MyScene(const std::string& littleNickname) : vrm::Scene(), m_LittleNickName(littleNickname) {}
	~MyScene() = default;

protected:
	void onInit() override
	{
		getApplication().getTrigger("Hey")
			.bindCallback(VRM_TRIGGER_CALLBACK(heyFunction));

		LOG_TRACE("MyScene \"{}\" instance initialized.", m_LittleNickName);
	}

	void onUpdate(float dt) override
	{
		m_TimeAccumulator += dt;
		m_FramesAccumulator++;

		if (m_TimeAccumulator < 1.f) return;

		LOG_TRACE("MyScene \"{}\" instance updated. fps: {}", m_LittleNickName, m_FramesAccumulator);

		m_FramesAccumulator = 0;
		m_TimeAccumulator -= 1.f;
	}

	void onRender() override
	{
	}

	void onEnd() override
	{
		LOG_TRACE("MyScene \"{}\" instance ended.", m_LittleNickName);
	}

	void heyFunction(bool activated) const
	{
		if (activated)
			LOG_INFO("Hey!");
	}

private:
	std::string m_LittleNickName;
	float m_TimeAccumulator = 0.f;
	size_t m_FramesAccumulator = 0;
};

int main(int argc, char** argv)
{
	vrm::Application app{argc, argv};
	
	app.createTrigger("MoveForward")
		.bindInput(vrm::KeyCode::W)
		.bindInput(vrm::KeyCode::Up)
		.bindCallback([](bool triggered) { LOG_INFO("Moving forward  : {}", triggered); });
	app.createTrigger("MoveBackward")
		.bindInput(vrm::KeyCode::S)
		.bindInput(vrm::KeyCode::Down)
		.bindCallback([](bool triggered) { LOG_INFO("Moving backward : {}", triggered); });
	app.createTrigger("MoveLeft")
		.bindInput(vrm::KeyCode::A)
		.bindInput(vrm::KeyCode::Left)
		.bindCallback([](bool triggered) { LOG_INFO("Moving left     : {}", triggered); });
	app.createTrigger("MoveRight")
		.bindInput(vrm::KeyCode::D)
		.bindInput(vrm::KeyCode::Right)
		.bindCallback([](bool triggered) { LOG_INFO("Moving right    : {}", triggered); });
	app.createTrigger("Hey")
		.bindInput(vrm::KeyCode::H);

	app.createCustomEvent("Exit")
		.bindInput(vrm::Event::Type::KeyPressed, vrm::KeyCode::Escape)
		.bindInput(vrm::Event::Type::Exit)
		.bindCallback([&app](const vrm::Event&) { LOG_INFO("Exit custom event."); app.exit(); });
	
	app.loadScene<MyScene>("A cute little scene !");

	app.run();

	return 0;
}
