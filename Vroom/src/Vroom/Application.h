#pragma once

#include "Core.h"

namespace Vroom {

	class VROOM_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	Application* CreateApplication();

}