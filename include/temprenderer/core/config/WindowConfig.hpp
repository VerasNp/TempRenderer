#pragma once
#include "toml++/impl/std_optional.hpp"
#include "toml++/impl/table.hpp"

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
  unsigned int width = 800;
  /**
   * Window height
   */
  unsigned int height = 600;

  static WindowConfig loadWindowConfig(const toml::table &table);
};
} // namespace temprenderer::core::config
