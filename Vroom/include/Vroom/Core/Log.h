/*
This logging system is widely inspired by TheCherno's "Hazel" game engine : https://github.com/TheCherno/Hazel .
Thanks to him for this powerful logging class.
*/

#pragma once

#include <spdlog/spdlog.h>

#include <memory>

namespace Vroom
{
	class Log
	{
	public:
		// Static class : we can delete constructor
		Log() = delete;

	public:
		static void init();

		static spdlog::logger& getLogger() { return *s_Logger; }
	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
	};
}


/* Logging macros */
#ifdef _DEBUG
#define LOG_TRACE(...)    Vroom::Log::getLogger().trace(__VA_ARGS__)
#define LOG_INFO(...)     Vroom::Log::getLogger().info(__VA_ARGS__)
#define LOG_WARN(...)     Vroom::Log::getLogger().warn(__VA_ARGS__)
#define LOG_ERROR(...)    Vroom::Log::getLogger().error(__VA_ARGS__)
#else
#define LOG_TRACE(...)
#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_ERROR(...)
#endif
