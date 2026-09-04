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

/**
 * @brief Possible log levels
 */
enum class LogLevel { DEBUG, INFO, WARNING, ERROR, UNKNOWN };

/**
 * @brief Logger system for application
 */
class LoggerManager : public ISubsystem {
public:
  void startUp() override;
  void shutDown() override;

  /**
   * @brief Outputs log message
   *
   * @param logLevel Log level of message
   * @param message Message of the log
   */
  static void log(LogLevel logLevel, const std::string &message);
  /**
   * @brief Outputs log message
   *
   * @pre Only verbose logs
   *
   * @param logLevel Log message level
   * @param message Message of the log
   */
  static void logVerbose(LogLevel logLevel, const std::string &message);
  /**
   * @brief Set if log manager will work with verbose logs
   *
   * @param verbose Verbose selector
   */
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
