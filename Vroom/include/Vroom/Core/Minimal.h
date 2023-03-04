#pragma once

#include <memory>
#include <functional>

#include "Vroom/Core/Assert.h"
#include "Vroom/Core/Log.h"

#ifdef _DEBUG
	#define IS_DEBUG() (true)
#else
	#define IS_DEBUG() (false)
#endif