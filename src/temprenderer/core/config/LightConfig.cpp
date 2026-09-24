#include "core/config/LightConfig.hpp"

#include "core/config/utils.hpp"
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
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Light config loaded successfully");
  return lightConfig;
}

LightType
LightConfig::stringToLightType(const std::string &lightType) noexcept {
  if (lightType == "point") {
    return LightType::POINT;
  }
  if (lightType == "ambient") {
    return LightType::AMBIENT;
  }
  LC_LOG(logging::LogLevel::WARNING, "Unknown light type: '" + lightType + "'");
  return LightType::UNKNOWN;
}
} // namespace temprenderer::core::config
