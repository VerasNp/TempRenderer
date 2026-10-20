#include "core/config/CameraConfig.hpp"
#include "core/config/utils.hpp"
#include "core/logging/LoggerManager.hpp"

namespace temprenderer::core::config {
[[nodiscard]] CameraConfig
CameraConfig::loadConfig(const parsers::ConfigValue &cameraConfig) noexcept {
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Loading camera config");
  CameraConfig config;
  config.name = cameraConfig.get("name")->asString("");
  if (const auto eyeConfig = cameraConfig.get("eye")) {
    config.eye = parsePoint3DDataFromConfig(*eyeConfig);
  }
  if (const auto lookAtConfig = cameraConfig.get("look_at")) {
    config.lookAt = parsePoint3DDataFromConfig(*lookAtConfig);
  }
  config.focalLength =
      cameraConfig.get("focal_length")->asFloat(config.focalLength);
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Camera config loaded successfully");
  return config;
}
} // namespace temprenderer::core::config
