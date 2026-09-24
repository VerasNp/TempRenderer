#pragma once
#include "core/math/Materials.hpp"
#include "core/parsers/ParserPort.hpp"
#include "kwp/Point3.hpp"

namespace temprenderer::core::config {
/**
 * Possible types of light
 */
enum class LightType { POINT, AMBIENT, UNKNOWN };

/**
 * @brief Light configs
 */
struct LightConfig {
  std::string name;
  LightType type = LightType::UNKNOWN;
  kwp::Point3 position = kwp::Point3(0, 0, 0);
  math::ColorF color = math::ColorF(1.0F, 1.0F, 1.0F);
  float intensity = 10.0F;

  [[nodiscard]] static LightConfig
  loadConfig(const parsers::ConfigValue &lightConfig) noexcept;

  /**
 * @brief TODO
 *
 * @param lightType
 * @return
 */
  [[nodiscard]] static LightType inline stringToLightType(
      const std::string &lightType) noexcept;
};
} // namespace temprenderer::core::config
