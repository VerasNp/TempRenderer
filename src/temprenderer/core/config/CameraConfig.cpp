#include "core/config/CameraConfig.hpp"

#include "core/config/utils.hpp"
#include "core/logging/LoggerManager.hpp"
namespace temprenderer::core::config {
[[nodiscard]] CameraConfig
CameraConfig::loadCameraConfig(const toml::table &table) noexcept {
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Loading camera config");
  CameraConfig config;
  if (auto *const eyeTable = table["eye"].as_table()) {
    config.eye = parsePoint3DDataFromConfig(*eyeTable);
  }
  config.focalLength = table["focal_length"].value_or(config.focalLength);
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Camera config loaded successfully");
  return config;
}
} // namespace temprenderer::core::config
