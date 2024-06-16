#include <iostream>

#include <Vroom/Core/Application.h>

int main(int argc, char** argv)
{
	auto app = std::make_unique<vrm::Application>(argc, argv);
	app->createTrigger("Exit")
		.bindInput(vrm::KeyCode::Escape)
		.bindCallback([&app](bool) { app->exit(); });
	
	app->createTrigger("test")
		.bindInput(vrm::KeyCode::W)
		.bindInput(vrm::KeyCode::A)
		.bindInput(vrm::KeyCode::S)
		.bindInput(vrm::KeyCode::D)
		.bindInput(vrm::MouseCode::Left)
		.bindCallback([](bool triggered) { LOG_INFO("Test trigger: {}", triggered); });

	app->run();

	app.release();

	return 0;
}