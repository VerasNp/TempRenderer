#pragma once
#include "kwp/Point3.hpp"
#include "toml++/toml.hpp"

namespace temprenderer::core::config {
/**
* @brief Camera configs
*/
struct CameraConfig {
  /**
   * Position on camera
   */
  kwp::Point3 eye{0, 0, 0};
  /**
   * Distance from canvas
   */
  kwp::Scalar focalLength = 1.0F;

  [[nodiscard]] static CameraConfig
  loadCameraConfig(const toml::table &table) noexcept;
};
} // namespace temprenderer::core::config
