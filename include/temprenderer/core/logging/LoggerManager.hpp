#pragma once

#include "temprenderer/ISubsystem.hpp"

#include <string>

#define RESET "\033[0m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"
#define CYAN "\033[36m"
#define BLUE "\033[94m"

namespace temprenderer::core::logging {

enum class LogLevel { DEBUG, INFO, WARNING, ERROR, UNKNOWN };

class LoggerManager : public ISubsystem {
public:
  void startUp() override;
  void shutDown() override;

  static void log(LogLevel logLevel, const std::string &message);
  static void logVerbose(LogLevel logLevel, const std::string &message);
  static void setVerbose(const bool verbose) { isVerbose = verbose; }

private:
  static std::string levelToString(LogLevel level);
  static inline bool isVerbose = false;
};

} // namespace temprenderer::core::logging

#define LC_LOG(level, msg)                                                     \
  temprenderer::core::logging::LoggerManager::log(level, msg)
#define LC_LOG_VERBOSE(level, msg)                                             \
  temprenderer::core::logging::LoggerManager::logVerbose(level, msg)
