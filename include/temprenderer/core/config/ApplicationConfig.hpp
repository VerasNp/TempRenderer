#pragma once

#include "RenderConfig.hpp"
#include "WindowConfig.hpp"
#include <string>

namespace temprenderer::core::config {

/**
 * @brief General application config
 */
struct ApplicationConfig {
  /**
   * Window configs
   */
  WindowConfig window;
  /**
   * Render configs
   */
  RenderConfig render;

  /**
   * @brief Loads application config from file
   *
   * @param applicationConfigFilePath Path to config file
   * @return General application config
   */
  [[nodiscard]] static ApplicationConfig
  loadFromFile(const std::string &applicationConfigFilePath);
};
} // namespace temprenderer::core::config
