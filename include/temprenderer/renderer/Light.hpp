#pragma once
#include "core/math/Materials.hpp"
#include "kwp/Point3.hpp"

namespace temprenderer::renderer {
class Light {
public:
  Light(core::config::LightType lightType, kwp::Point3 position,
        core::math::ColorF color, kwp::Scalar intensity)
      : lightType_(lightType), position_(position), color_(color),
        intensity_(intensity) {}
  ~Light() = default;
  /**
   * @brief TODO
   *
   * @return
   */
  [[nodiscard]] kwp::Point3 getPosition() const noexcept {
    return this->position_;
  }

  /**
   * @brief TODO
   *
   * @return
   */
  [[nodiscard]] core::math::ColorF getColor() const noexcept {
    return this->color_;
  }

  /**
   * @brief TODO
   *
   * @return
   */
  [[nodiscard]] kwp::Scalar getIntensity() const noexcept {
    return this->intensity_;
  }

  [[nodiscard]] core::config::LightType getLightType() const noexcept {
    return this->lightType_;
  }

private:
  core::config::LightType lightType_;
  kwp::Point3 position_;
  core::math::ColorF color_;
  kwp::Scalar intensity_;
};
} // namespace temprenderer::renderer
