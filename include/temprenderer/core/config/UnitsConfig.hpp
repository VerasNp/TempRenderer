#pragma once

#include "core/parsers/ParserPort.hpp"

#include <string>

namespace temprenderer::core::config {
enum class LengthTypes { METER, KILOMETER, CENTIMETER, MILLIMETER, UNKNOWN };
struct UnitsConfig {
  /**
   * @brief TODO
   */
  LengthTypes length;
  /**
   * @brief TODO
   *
   * @param cameraConfig
   * @return
   */
  [[nodiscard]] static UnitsConfig
  loadConfig(const parsers::ConfigValue &cameraConfig) noexcept;

  [[nodiscard]] static LengthTypes
  stringToLengthType(const std::string &lengthType) noexcept;
};
} // namespace temprenderer::core::config
