#include "Vroom/Core/Log.h"

#pragma warning(push, 0)
#include <spdlog/sinks/stdout_color_sinks.h>
#pragma warning(pop)

namespace Vroom
{
	std::shared_ptr<spdlog::logger> Log::s_Logger;

	void Log::init()
	{
		spdlog::sink_ptr logSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		logSink->set_pattern("%^[%T] %n: %v%$");

		s_Logger = std::make_shared<spdlog::logger>("APPLICATION", logSink);

		spdlog::register_logger(s_Logger);
		s_Logger->set_level(spdlog::level::trace);
		s_Logger->flush_on(spdlog::level::trace);
	}
}
