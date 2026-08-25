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
  LoggerManager() = default;
  virtual ~LoggerManager() = default;

  void startUp() override;
  void shutDown() override;

  static void log(LogLevel logLevel, const std::string &message);

private:
  static std::string levelToString(LogLevel level);
};
#define LC_LOG(level, msg)                                                     \
  temprenderer::core::logging::LoggerManager::log(level, msg)

} // namespace temprenderer::core::logging
