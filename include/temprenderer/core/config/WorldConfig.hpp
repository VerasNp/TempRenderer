#pragma once
#include "core/math/Materials.hpp"
#include "core/parsers/ParserPort.hpp"

namespace temprenderer::core::config {

struct BackgroundConfig {
  math::ColorF color;
};

struct WorldConfig {
  BackgroundConfig background;

  /**
   * @brief TODO
   *
   * @param config
   * @return
   */
  [[nodiscard]]
  static WorldConfig loadConfig(const parsers::ConfigValue &config);
};
} // namespace temprenderer::core::config
