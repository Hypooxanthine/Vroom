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
#define VRM_CRASH() \
    LOG_CRITICAL("Application crash has been requested at file {}, line {}.", std::filesystem::path(__FILE__).filename().string(), __LINE__); \
    VRM_CRASH_NO_MSG()

/** 
 * @brief Vroom assertion with critical log if failed and application crash.
 */
#define VRM_ASSERT(x) \
    if(!(x)) \
    { \
        LOG_CRITICAL("Assertion \"{}\" failed at file {}, line {}.", #x, std::filesystem::path(__FILE__).filename().string(), __LINE__); \
        VRM_CRASH_NO_MSG(); \
    }
