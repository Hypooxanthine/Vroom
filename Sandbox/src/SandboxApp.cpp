#include <Vroom.h>

class Sandbox : public Vroom::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

Vroom::Application* Vroom::CreateApplication()
{
	return new Sandbox();
}