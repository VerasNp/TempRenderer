#pragma once
#include "core/config/SceneConfig.hpp"
#include "core/math/Materials.hpp"
#include "kwp/Point3.hpp"

namespace temprenderer::renderer {
class Light {
public:
  Light(std::string name, core::config::LightType lightType,
        kwp::Point3 position, core::math::ColorF color, kwp::Scalar intensity,
        kwp::Scalar width = 0.0F, kwp::Scalar height = 0.0F)
      : name_(name), lightType_(lightType), position_(position), color_(color),
        intensity_(intensity), width_(width), height_(height) {}
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

  [[nodiscard]] kwp::Point3 samplePosition(float u, float v) const noexcept {
    return position_ +
           kwp::Vec3{(u - 0.5F) * width_, (v - 0.5F) * height_, 0.0F};
  }

  [[nodiscard]] bool isAreaLight() const noexcept {
    return lightType_ == core::config::LightType::AREA && width_ > 0.0F &&
           height_ > 0.0F;
  }

private:
  std::string name_;
  core::config::LightType lightType_;
  kwp::Point3 position_;
  core::math::ColorF color_;
  kwp::Scalar intensity_;
  kwp::Scalar width_;
  kwp::Scalar height_;
};
} // namespace temprenderer::renderer
