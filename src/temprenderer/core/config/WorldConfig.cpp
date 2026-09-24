#include "core/config/WorldConfig.hpp"
#include "core/config/utils.hpp"
#include "core/logging/LoggerManager.hpp"

namespace temprenderer::core::config {
WorldConfig WorldConfig::loadConfig(const parsers::ConfigValue &config) {
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Loading world config");
  WorldConfig worldConfig;
  worldConfig.background.color =
      parseColorDataFromConfig(*(config.get("background"))->get("color"));
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "World config loaded successfully");
  return worldConfig;
}
} // namespace temprenderer::core::config
