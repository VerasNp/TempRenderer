#include "temprenderer/core/logging/LoggerManager.hpp"

#include <ctime>
#include <iostream>

namespace temprenderer::core::logging {
static bool isLoggerInit = false;

void LoggerManager::startUp() {
  if (!isLoggerInit) {
    isLoggerInit = true;
    LC_LOG(LogLevel::INFO, "Starting up logger");
  }
}

void LoggerManager::shutDown() {
  if (isLoggerInit) {
    LC_LOG(LogLevel::INFO, "Shutting down logger");
    isLoggerInit = false;
  }
}

static std::string getTimestamp() {
  const std::time_t now = std::time(nullptr);
  std::tm timeinfo{};

#if defined(_WIN32) || defined(_WIN64)
  localtime_s(&timeinfo, &now);
#else
  localtime_r(&now, &timeinfo);
#endif

  char timestamp[20];
  std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return std::string(timestamp);
}

void LoggerManager::log(const LogLevel logLevel, const std::string &message) {
  if (isLoggerInit) {
    std::cout << "[" << getTimestamp() << "] " << levelToString(logLevel)
              << ": " << message << "\n";
  }
}

std::string LoggerManager::levelToString(const LogLevel level) {
  switch (level) {
  case LogLevel::DEBUG:
    return CYAN "[DEBUG]" RESET;
  case LogLevel::INFO:
    return BLUE "[INFO]" RESET;
  case LogLevel::WARNING:
    return YELLOW "[WARNING]" RESET;
  case LogLevel::ERROR:
    return RED "[ERROR]" RESET;
  default:
    return "[UNKNOWN]";
  }
}
} // namespace temprenderer::core::logging
