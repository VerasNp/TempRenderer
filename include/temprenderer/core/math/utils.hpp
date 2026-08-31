#pragma once
#include <complex>
#include <kwp/kwp_config.hpp>
namespace temprenderer::core::math {
/**
 * @brief Solves a quadratic equation of the form ax² + bx + c = 0.
 *
 * @param a Quadratic coefficient. Must be non-zero.
 * @param b Linear coefficient.
 * @param c Constant coefficient.
 * @param t0 Pointer to the first root.
 * @param t1 Pointer to the second root.
 * @return `true` if the equation has real roots, `false` if the discriminant is negative.
 */
inline bool quadratic(const kwp::Scalar a, const kwp::Scalar b,
                      const kwp::Scalar c, kwp::Scalar *t0,
                      kwp::Scalar *t1) noexcept {
  const double discriminant =
      (static_cast<double>(b) * static_cast<double>(b)) -
      (4 * static_cast<double>(a) * static_cast<double>(c));
  if (discriminant < 0) {
    return false;
  }
  const double rootDiscriminant = std::sqrt(discriminant);
  double q;
  if (b < 0) {
    q = -.5 * (b - rootDiscriminant);
  } else {
    q = -.5 * (b + rootDiscriminant);
  }
  *t0 = q / a;
  *t1 = c / q;
  if (*t0 > *t1) {
    std::swap(*t0, *t1);
  }
  return true;
}
} // namespace temprenderer::core::math
