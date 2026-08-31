#pragma once
#include "core/config/ApplicationConfig.hpp"
#include "kwp/Point3.hpp"
#include "temprenderer/core/math/Ray.hpp"

namespace temprenderer::renderer {
/**
 * @brief Represents the camera used to generate rays for rendering.
 */
class Camera {
public:
  Camera(const kwp::Point3 &eye, unsigned int resolutionWidth,
         unsigned int resolutionHeight, float viewportWidth,
         float viewportHeight);
  /**
   * @brief Generates a ray passing through a pixel.
   *
   * @param col Horizontal pixel coordinate.
   * @param row Vertical pixel coordinate.
   * @return The ray corresponding to the specified pixel
   */
  core::math::Ray generateRay(unsigned int col,
                              unsigned int row) const noexcept;

private:
  kwp::Point3 eye_;
  float viewPortPixelDx_;
  float viewPortPixelDy_;
  kwp::Point3 viewPortUpperLeft_;
  core::config::CameraConfig cameraConfig_;
  float resolutionWidth_;
  float resolutionHeight_;
};
} // namespace temprenderer::renderer
