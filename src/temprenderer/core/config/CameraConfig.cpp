#include "core/config/CameraConfig.hpp"
#include "core/config/utils.hpp"
#include "core/logging/LoggerManager.hpp"

namespace temprenderer::core::config {
[[nodiscard]] CameraConfig CameraConfig::loadCameraConfig(
    const parsers::ConfigValue &cameraConfig) noexcept {
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Loading camera config");
  CameraConfig config;
  if (const auto eyeConfig = cameraConfig.get("eye")) {
    config.eye = parsePoint3DDataFromConfig(*eyeConfig);
  }
  config.focalLength =
      cameraConfig.get("focal_length")->asFloat(config.focalLength);
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Camera config loaded successfully");
  return config;
}
} // namespace temprenderer::core::config
