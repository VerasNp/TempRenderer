#pragma once
#include "core/math/Materials.hpp"
#include "kwp/Point3.hpp"

namespace temprenderer::renderer {
class Light {
public:
  Light(kwp::Point3 position, core::math::Color color, kwp::Scalar intensity)
      : position_(position), color_(color), intensity_(intensity) {}
  ~Light() = default;
  [[nodiscard]] kwp::Point3 getPosition() const noexcept {
    return this->position_;
  }

private:
  kwp::Point3 position_;
  core::math::Color color_;
  kwp::Scalar intensity_;
};
} // namespace temprenderer::renderer
