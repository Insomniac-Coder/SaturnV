#ifndef LOG_H
#define LOG_H
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <chrono>
#include <ctime>
#include <string>
#include <Windows.h>

namespace Saturn {

#ifdef __unix__

	enum class ErrorType {
		FAIL = 31,
		PASS = 32,
		WARN = 33,
		INFO = 0
	};

#elif defined(_WIN32) || defined(_WIN64)

	enum class ErrorType {
		FAIL = FOREGROUND_RED,
		PASS = FOREGROUND_GREEN,
		WARN = FOREGROUND_RED | FOREGROUND_GREEN,
		INFO = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
	};

#endif

	class Logger {
	private:
		static const std::string& loggerId;
		static std::string GetTime();
	public:
		static void Log(std::string message, ErrorType type, const char* file, const int line);
	};
}

#define LOG_FAIL(...) Saturn::Logger::Log(__VA_ARGS__, Saturn::ErrorType::FAIL, __FILE__, __LINE__)
#define LOG_WARN(...) Saturn::Logger::Log(__VA_ARGS__, Saturn::ErrorType::WARN, __FILE__, __LINE__)
#define LOG_INFO(...) Saturn::Logger::Log(__VA_ARGS__, Saturn::ErrorType::INFO, __FILE__, __LINE__)
#define LOG_PASS(...) Saturn::Logger::Log(__VA_ARGS__, Saturn::ErrorType::PASS, __FILE__, __LINE__)

#endif LOG_H

