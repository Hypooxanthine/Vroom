#pragma once

#include <spdlog/logger.h>
#include <spdlog/spdlog.h>

#include "Vroom/Api.h"

namespace vrm
{

/**
 * @brief Static class for logging. Needs to be initialized before calling logging macros.
 */
class VRM_API Log
{
public:

  /**
   * @brief Sets logging level and display pattern.
   */
  static void Init();

  static spdlog::logger& GetLogger();

private:

  static spdlog::logger s_logger;
};

} // namespace vrm

/**
 * @brief Trace level logging.
 * Calling Log::Init() is required before usage.
 */
#define VRM_LOG_TRACE(...) ::vrm::Log::GetLogger().trace(__VA_ARGS__)

/**
 * @brief Info level logging.
 * Calling Log::Init() is required before usage.
 */
#define VRM_LOG_INFO(...) ::vrm::Log::GetLogger().info(__VA_ARGS__)

/**
 * @brief Warning level logging.
 * Calling Log::Init() is required before usage.
 */
#define VRM_LOG_WARN(...) ::vrm::Log::GetLogger().warn(__VA_ARGS__)

/**
 * @brief Error level logging.
 * Calling Log::Init() is required before usage.
 */
#define VRM_LOG_ERROR(...) ::vrm::Log::GetLogger().error(__VA_ARGS__)

/**
 * @brief Critical level logging.
 * Calling Log::Init() is required before usage.
 */
#define VRM_LOG_CRITICAL(...) ::vrm::Log::GetLogger().critical(__VA_ARGS__)
