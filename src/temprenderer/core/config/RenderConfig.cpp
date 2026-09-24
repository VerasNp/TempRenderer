#include "core/config/RenderConfig.hpp"
#include "core/config/utils.hpp"
#include "core/logging/LoggerManager.hpp"
#include <string>

namespace temprenderer::core::config {
RenderConfig
RenderConfig::loadConfig(const parsers::ConfigValue &renderConfig) {
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Loading render config");
  RenderConfig config;
  config.resolutionWidth =
      renderConfig.get("resolution_width")->asFloat(config.resolutionWidth);
  config.aspectRatio =
      stringToAspectRatio(renderConfig.get("aspect_ratio")->asString());
  config.resolutionHeight =
      calculateResolutionHeight(config.aspectRatio, config.resolutionWidth);
  config.viewportHeight =
      renderConfig.get("viewport_height")->asFloat(config.viewportHeight);
  config.viewportWidth = calculateViewportWidth(
      config.resolutionWidth, config.resolutionHeight, config.viewportHeight);
  config.shadows = renderConfig.get("shadows")->asBool(config.shadows);
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Render config loaded successfully");
  return config;
}
} // namespace temprenderer::core::config
