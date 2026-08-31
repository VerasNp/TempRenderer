#pragma once
#include "kwp/Point3.hpp"
#include "kwp/Vec3.hpp"

namespace temprenderer::core::math {
/**
 * @brief Manages ray component
 */
class Ray {
public:
  Ray() = default;
  Ray(const kwp::Point3 &origin, const kwp::Vec3 &direction)
      : origin_(origin), direction_(direction) {};

  /**
   * @brief calculates point in the ray at scalar t
   *
   * @param t Scalar to variate the ray
   * @return Point at "t"
   */
  constexpr kwp::Point3 operator()(const kwp::Scalar t) const noexcept {
    return origin_ + (direction_ * t);
  }

private:
  kwp::Point3 origin_;
  kwp::Vec3 direction_;
};
} // namespace temprenderer::core::math
