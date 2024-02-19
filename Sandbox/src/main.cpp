#include <iostream>

#include <Vroom/Core/Application.h>

int main(int argc, char** argv)
{
	auto app = std::make_unique<vrm::Application>(argc, argv);
	
	app->run();

	app.release();

	return 0;
}