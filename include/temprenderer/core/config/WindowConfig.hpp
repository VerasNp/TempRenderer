#pragma once
#include <cstdint>
#include <string>
#include <toml++/toml.hpp>

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
   * @param table Window TOML table
   * @return Window config struct
   */
  [[nodiscard]] static WindowConfig loadWindowConfig(const toml::table &table);
};
} // namespace temprenderer::core::config
