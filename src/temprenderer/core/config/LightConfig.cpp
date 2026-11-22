#include "core/config/LightConfig.hpp"

#include "core/config/utils.hpp"

#include <algorithm>

namespace temprenderer::core::config {
[[nodiscard]] LightConfig LightConfig::loadConfig(
    const parsers::ConfigValue &parsedLightConfig) noexcept {
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Loading light config");
  LightConfig lightConfig;
  lightConfig.type =
      stringToLightType(parsedLightConfig.get("type")->asString());
  if (const auto positionConfigValue = parsedLightConfig.get("position")) {
    lightConfig.position = parsePoint3DDataFromConfig(*positionConfigValue);
  }
  if (const auto colorConfigValue = parsedLightConfig.get("color")) {
    lightConfig.color = parseColorDataFromConfig(*colorConfigValue);
  }
  lightConfig.intensity = parsedLightConfig.get("intensity")->asFloat();
  if (const auto widthConfig = parsedLightConfig.get("width")) {
    lightConfig.width = std::max(widthConfig->asFloat(), 0.0F);
  }
  if (const auto heightConfig = parsedLightConfig.get("height")) {
    lightConfig.height = std::max(heightConfig->asFloat(), 0.0F);
  }
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Light config loaded successfully");
  return lightConfig;
}

LightType
LightConfig::stringToLightType(const std::string &lightType) noexcept {
  if (lightType == "point") {
    return LightType::POINT;
  }
  if (lightType == "area") {
    return LightType::AREA;
  }
  if (lightType == "ambient") {
    return LightType::AMBIENT;
  }
  LC_LOG(logging::LogLevel::WARNING, "Unknown light type: '" + lightType + "'");
  return LightType::UNKNOWN;
}
} // namespace temprenderer::core::config
