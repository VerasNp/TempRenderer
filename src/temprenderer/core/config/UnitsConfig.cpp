#include "core/config/UnitsConfig.hpp"
namespace temprenderer::core::config {
[[nodiscard]] UnitsConfig
UnitsConfig::loadConfig(const parsers::ConfigValue &cameraConfig) noexcept {
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Loading units config");
  UnitsConfig config;
  config.length = stringToLengthType(cameraConfig.get("length")->asString());
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Units config loaded successfully");
  return config;
}
LengthTypes
UnitsConfig::stringToLengthType(const std::string &lengthType) noexcept {
  if (lengthType == "m") {
    return LengthTypes::METER;
  }
  if (lengthType == "km") {
    return LengthTypes::KILOMETER;
  }
  if (lengthType == "cm") {
    return LengthTypes::CENTIMETER;
  }
  if (lengthType == "mm") {
    return LengthTypes::MILLIMETER;
  }
  LC_LOG(logging::LogLevel::WARNING,
         "Unknown length type '" + lengthType + "'");
  return LengthTypes::UNKNOWN;
}
} // namespace temprenderer::core::config
