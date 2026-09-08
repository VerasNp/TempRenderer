#include "core/config/RenderConfig.hpp"
#include "core/config/utils.hpp"
#include "core/logging/LoggerManager.hpp"
#include <string>

namespace temprenderer::core::config {
RenderConfig RenderConfig::loadRenderConfig(const toml::table &table) {
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Loading render config");
  RenderConfig config;
  config.resolutionWidth =
      table["resolution_width"].value_or(config.resolutionWidth);
  if (const auto aspectStr = table["aspect_ratio"].value<std::string>();
      !aspectStr) {
    config.aspectRatio = stringToAspectRatio(*aspectStr);
  }
  config.resolutionHeight =
      calculateResolutionHeight(config.aspectRatio, config.resolutionWidth);
  config.viewportHeight =
      table["viewport_height"].value_or(config.viewportHeight);
  config.viewportWidth = calculateViewportWidth(
      config.resolutionWidth, config.resolutionHeight, config.viewportHeight);
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Render config loaded successfully");
  return config;
}
} // namespace temprenderer::core::config
