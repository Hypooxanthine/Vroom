#include <iostream>

#include <Vroom/Core/Application.h>

int main(int argc, char** argv)
{
	auto app = std::make_unique<vrm::Application>(argc, argv);
	
	app->createTrigger("MoveForward")
		.bindInput(vrm::KeyCode::W)
		.bindInput(vrm::KeyCode::Up)
		.bindCallback([](bool triggered) { LOG_INFO("Moving forward  : {}", triggered); });
	app->createTrigger("MoveBackward")
		.bindInput(vrm::KeyCode::S)
		.bindInput(vrm::KeyCode::Down)
		.bindCallback([](bool triggered) { LOG_INFO("Moving backward : {}", triggered); });
	app->createTrigger("MoveLeft")
		.bindInput(vrm::KeyCode::A)
		.bindInput(vrm::KeyCode::Left)
		.bindCallback([](bool triggered) { LOG_INFO("Moving left     : {}", triggered); });
	app->createTrigger("MoveRight")
		.bindInput(vrm::KeyCode::D)
		.bindInput(vrm::KeyCode::Right)
		.bindCallback([](bool triggered) { LOG_INFO("Moving Right    : {}", triggered); });

	app->createCustomEvent("Exit")
		.bindInput(vrm::Event::Type::KeyPressed, vrm::KeyCode::Escape)
		.bindInput(vrm::Event::Type::Exit)
		.bindCallback([&app](const vrm::Event&) { LOG_INFO("Exit custom event."); app->exit(); });

	app->run();

	app.release();

	return 0;
}