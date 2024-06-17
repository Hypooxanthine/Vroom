#include <iostream>

#include <Vroom/Core/Application.h>

int main(int argc, char** argv)
{
	auto app = std::make_unique<vrm::Application>(argc, argv);
	
	app->createTrigger("test")
		.bindInput(vrm::KeyCode::W)
		.bindInput(vrm::KeyCode::A)
		.bindInput(vrm::KeyCode::S)
		.bindInput(vrm::KeyCode::D)
		.bindInput(vrm::MouseCode::Left)
		.bindCallback([](bool triggered) { LOG_INFO("Test trigger: {}", triggered); });

	app->createCustomEvent("Exit")
		.bindInput(vrm::Event::Type::KeyPressed, vrm::KeyCode::Escape)
		.bindInput(vrm::Event::Type::Exit)
		.bindCallback([&app](const vrm::Event&) { LOG_INFO("Exit custom event."); app->exit(); });

	app->run();

	app.release();

	return 0;
}