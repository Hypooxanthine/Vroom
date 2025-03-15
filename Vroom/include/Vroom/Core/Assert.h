#pragma once

#include "Vroom/Core/Log.h"
#include <filesystem>

/**
 * @brief Request application crash with no message.
 */
#define VRM_CRASH_NO_MSG() throw std::runtime_error("Application crash has been requested.")

/**
 * @brief Request application crash. Displays a critical log with file and line where crash has been requested.
 */
#define VRM_CRASH()                                                                                                                             \
  VRM_LOG_CRITICAL("Application crash has been requested at file {}, line {}.", std::filesystem::path(__FILE__).filename().string(), __LINE__); \
  VRM_CRASH_NO_MSG()

/**
 * @brief Vroom assertion. Makes application crash if condition is false.
 */
#define VRM_ASSERT(x)                                                                                                                    \
  if (!(x))                                                                                                                              \
  {                                                                                                                                      \
    VRM_LOG_CRITICAL("Assertion \"{}\" failed at file {}, line {}.", #x, std::filesystem::path(__FILE__).filename().string(), __LINE__); \
    VRM_CRASH_NO_MSG();                                                                                                                  \
  }

/**
 * @brief Vroom assertion with critical log if failed and application crash.
 *
 */
#define VRM_ASSERT_MSG(x, ...)                                                                                                                    \
  if (!(x))                                                                                                                                       \
  {                                                                                                                                               \
    VRM_LOG_CRITICAL("Assertion \"{}\" failed at file {}, line {}. Message:", #x, std::filesystem::path(__FILE__).filename().string(), __LINE__); \
    VRM_LOG_CRITICAL(__VA_ARGS__);                                                                                                                \
    VRM_CRASH_NO_MSG();                                                                                                                           \
  }

#ifdef VRM_DEBUG
#define VRM_DEBUG_ASSERT(x) VRM_ASSERT(x)
#else
#define VRM_DEBUG_ASSERT(x)
#endif

#ifdef VRM_DEBUG
#define VRM_DEBUG_ASSERT_MSG(x, ...) VRM_ASSERT_MSG(x, __VA_ARGS__)
#else
#define VRM_DEBUG_ASSERT_MSG(x, ...)
#endif

#define VRM_CHECK_FAIL \
  VRM_LOG_ERROR("Check failed at file {}, line {}.", std::filesystem::path(__FILE__).filename().string(), __LINE__);

#define VRM_CHECK_RETVAL(x, retVal) \
  if (!(x))                         \
  {                                 \
    VRM_CHECK_FAIL                  \
    return retVal;                  \
  }

#define VRM_CHECK_RETVAL_MSG(x, retVal, ...) \
  if (!(x))                                  \
  {                                          \
    VRM_CHECK_FAIL                           \
    VRM_LOG_ERROR(__VA_ARGS__);              \
    return retVal;                           \
  }

#define VRM_CHECK_RET(x) VRM_CHECK_RETVAL(x)

#define VRM_CHECK_RET_MSG(x, ...) VRM_CHECK_RETVAL_MSG(x, , __VA_ARGS__)

#define VRM_CHECK_RET_FALSE(x) VRM_CHECK_RETVAL(x, false)

#define VRM_CHECK_RET_FALSE_MSG(x, ...) VRM_CHECK_RETVAL_MSG(x, false, __VA_ARGS__)
