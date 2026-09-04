#pragma once

#include "CameraConfig.hpp"
#include "RenderConfig.hpp"
#include "SceneConfig.hpp"
#include "WindowConfig.hpp"
#include "kwp/Point3.hpp"
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
   * Camera config
   */
  CameraConfig camera;
  /**
   * Scene config
   */
  SceneConfig scene;

  /**
   * @brief Loads application config from file
   *
   * @param path Path to config file
   * @return General application config
   */
  [[nodiscard]] static ApplicationConfig loadFromFile(const std::string &path);
};
} // namespace temprenderer::core::config
