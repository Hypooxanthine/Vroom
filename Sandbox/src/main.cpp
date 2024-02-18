#include <iostream>

#include <Vroom/Core/Application.h>
#include <Vroom/Core/Log.h>

int main(int argc, char** argv)
{
	auto app = std::make_unique<vrm::Application>(argc, argv);
	
	app->run();

	return 0;
}