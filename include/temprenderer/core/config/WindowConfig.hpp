#pragma once
#include "core/parsers/ParserPort.hpp"

#include <cstdint>
#include <string>

namespace temprenderer::core::config {
/**
 * @brief Window configs
 */
struct WindowConfig {
  /**
   * Window title
   */
  std::string title = "TempRenderer Engine";
  /**
   * Window width
   */
  std::uint16_t width = 1280;
  /**
   * Window height
   */
  std::uint16_t height = 720;

  /**
   * @brief Loads window config into struct data structure
   *
   * @param windowConfig TODO
   * @return Window config struct
   */
  [[nodiscard]] static WindowConfig
  loadWindowConfig(const parsers::ConfigValue &windowConfig);
};
} // namespace temprenderer::core::config
