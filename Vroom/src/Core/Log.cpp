#include "Vroom/Core/Log.h"
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>


using namespace vrm;

spdlog::logger Log::s_logger("main_logger", std::make_shared<spdlog::sinks::stdout_color_sink_mt>());

void Log::Init()
{
  s_logger.set_level(spdlog::level::level_enum::trace);
  s_logger.set_pattern("[%H:%M:%S] %^[%l] %v%$");
}

spdlog::logger& Log::GetLogger()
{
  return s_logger;
}
