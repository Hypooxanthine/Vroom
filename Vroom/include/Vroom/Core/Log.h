#pragma once

#include <spdlog/spdlog.h>
#include "Vroom/Api.h"

/**
 * @brief Static class for logging. Needs to be initialized before calling logging macros.
 */
class VRM_API Log
{
public:
    /**
     * @brief Sets logging level and display pattern.
     */
    static void Init()
    {
        spdlog::set_level(spdlog::level::level_enum::trace);
        spdlog::set_pattern("[%H:%M:%S] %^[%l] %v%$");
    }
};

/**
 * @brief Trace level logging.
 * Calling Log::Init() is required before usage.
 */
#define VRM_LOG_TRACE(...) spdlog::trace(__VA_ARGS__)

 /**
  * @brief Info level logging.
  * Calling Log::Init() is required before usage.
  */
#define VRM_LOG_INFO(...) spdlog::info(__VA_ARGS__)

 /**
  * @brief Warning level logging.
  * Calling Log::Init() is required before usage.
  */
#define VRM_LOG_WARN(...) spdlog::warn(__VA_ARGS__)

 /**
  * @brief Error level logging.
  * Calling Log::Init() is required before usage.
  */
#define VRM_LOG_ERROR(...) spdlog::error(__VA_ARGS__)

 /**
  * @brief Critical level logging.
  * Calling Log::Init() is required before usage.
  */
#define VRM_LOG_CRITICAL(...) spdlog::critical(__VA_ARGS__)