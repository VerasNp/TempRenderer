#include "core/config/WindowConfig.hpp"

#include "core/logging/LoggerManager.hpp"
namespace temprenderer::core::config {
WindowConfig WindowConfig::loadWindowConfig(const toml::table &table) {
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Loading window config");
  WindowConfig config;
  config.title = table["title"].value_or(config.title);
  config.width = table["width"].value_or(config.width);
  config.height = table["height"].value_or(config.height);
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Window config loaded successfully");
  return config;
}
} // namespace temprenderer::core::config
