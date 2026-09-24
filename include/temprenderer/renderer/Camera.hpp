#pragma once
#include "core/config/ApplicationConfig.hpp"
#include "core/config/CameraConfig.hpp"
#include "kwp/Point3.hpp"
#include "temprenderer/core/math/Ray.hpp"

namespace temprenderer::renderer {
/**
 * @brief Represents the camera used to generate rays for rendering.
 */
class Camera {
public:
  Camera(const kwp::Point3 &eye, float focalLength,
         std::uint16_t resolutionWidth, std::uint16_t resolutionHeight,
         float viewportWidth, float viewportHeight);
  /**
   * @brief Generates a ray passing through a pixel.
   *
   * @param col Horizontal pixel coordinate.
   * @param row Vertical pixel coordinate.
   * @return The ray corresponding to the specified pixel
   */
  core::math::Ray generateRay(unsigned int col,
                              unsigned int row) const noexcept;

  /**
   * @brief Set new value to camera eye
   *
   * @param eye New Position
   */
  void setEye(const kwp::Point3 &eye) { this->eye_ = eye; }

  /**
   * @brief Get camera position
   *
   *
   * @return Camera Position
   */
  [[nodiscard]] kwp::Point3 getEye() const noexcept { return this->eye_; }

private:
  kwp::Point3 eye_;
  float viewPortPixelDx_;
  float viewPortPixelDy_;
  kwp::Point3 viewPortUpperLeft_;
  float focalLength_;
  float resolutionWidth_;
  float resolutionHeight_;
};
} // namespace temprenderer::renderer
