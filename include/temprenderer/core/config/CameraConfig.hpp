#pragma once
#include "core/parsers/ParserPort.hpp"
#include "kwp/Point3.hpp"

namespace temprenderer::core::config {
/**
* @brief Camera configs
*/
struct CameraConfig {
  /**
   * Component name
   */
  std::string name;
  /**
   * Position on camera
   */
  kwp::Point3 eye{0, 0, 0};
  /**
   * Distance from viewport
   */
  float focalLength = 1.0F;

  /**
   * @brief Loads camera config into struct
   *
   * @param cameraConfig TODO
   * @return Camera config
   */
  [[nodiscard]] static CameraConfig
  loadConfig(const parsers::ConfigValue &cameraConfig) noexcept;
};
} // namespace temprenderer::core::config
