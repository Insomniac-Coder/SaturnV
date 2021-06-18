#include "Log.h"

const std::string& Saturn::Logger::loggerId = "Saturn";

#ifdef __unix__

void Saturn::Logger::Log(std::string message, ErrorType type) {
	std::string code = std::to_string((int)type);
	printf("\033[%sm[%s][%s] %s\n", code.c_str(), GetTime().c_str(), loggerId, message.c_str());
	if (type == ErrorType::FAIL) {
		printf("\033%smAt line %d, in file %s\n", code.c_str(), line, file);
	}
}

#elif defined(_WIN32) || defined(_WIN64)

void Saturn::Logger::Log(std::string message, ErrorType type, const char* file, const int line) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (WORD)type);
	printf("[%s][%s] %s\n", GetTime().c_str(), loggerId.c_str(), message.c_str());
	if (type == ErrorType::FAIL) {
		printf("At line %d, in file %s\n", line, file);
	}
	SetConsoleTextAttribute(hConsole, (WORD)ErrorType::INFO);
}

#endif

std::string Saturn::Logger::GetTime() {
	auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::string time = (std::string)ctime(&timenow);
	return time.substr(0, time.size() - 1);
}