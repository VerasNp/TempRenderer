#include "core/config/LightConfig.hpp"

#include "core/config/utils.hpp"
namespace temprenderer::core::config {
[[nodiscard]] LightConfig LightConfig::loadLightConfig(
    const parsers::ConfigValue &parsedLightConfig) noexcept {
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Loading light config");
  LightConfig lightConfig;
  lightConfig.type =
      stringToLightType(parsedLightConfig.get("type")->asString());
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Light config loaded successfully");
}

LightType
LightConfig::stringToLightType(const std::string &lightType) noexcept {
  if (lightType == "point") {
    return LightType::POINT;
  }
  if (lightType == "ambient") {
    return LightType::AMBIENT;
  }
  LC_LOG_VERBOSE(logging::LogLevel::WARNING, "Unknown light type");
}
} // namespace temprenderer::core::config
