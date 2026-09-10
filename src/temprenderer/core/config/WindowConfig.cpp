#include "core/config/WindowConfig.hpp"
#include "core/logging/LoggerManager.hpp"
#include "core/parsers/ParserPort.hpp"

namespace temprenderer::core::config {
WindowConfig
WindowConfig::loadWindowConfig(const parsers::ConfigValue &windowConfig) {
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Loading window config");
  WindowConfig config;
  config.title = windowConfig.get("title")->asString(config.title);
  config.width = windowConfig.get("width")->asInt<std::uint16_t>(config.width);
  config.height =
      windowConfig.get("height")->asInt<std::uint16_t>(config.height);
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Window config loaded successfully");
  return config;
}
} // namespace temprenderer::core::config
