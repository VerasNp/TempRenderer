#pragma once
#include "kwp/Point3.hpp"
#include "kwp/Vec3.hpp"

namespace temprenderer::core::math {
class Ray {
public:
  Ray() = default;
  Ray(const kwp::Point3 &origin, const kwp::Vec3 &direction)
      : origin_(origin), direction_(direction) {};

  constexpr kwp::Point3 operator()(const kwp::Scalar t) const noexcept {
    return origin_ + (direction_ * t);
  }

  kwp::Point3 origin_;
  kwp::Vec3 direction_;
};
} // namespace temprenderer::core::math
