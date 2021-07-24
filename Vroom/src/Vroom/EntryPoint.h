#pragma once

#ifdef VR_PLATFORM_WINDOWS

extern Vroom::Application* Vroom::CreateApplication();

int main(int argc, char** argv)
{
	printf("Vroom Engine");

	auto application = Vroom::CreateApplication();

	application->Run();

	delete application;
}

#endif