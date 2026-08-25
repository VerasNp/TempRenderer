#pragma once
#include <complex>
#include <kwp/kwp_config.hpp>
namespace temprenderer::core::math {
inline bool quadratic(kwp::Scalar a, kwp::Scalar b, kwp::Scalar c,
                      kwp::Scalar *t0, kwp::Scalar *t1) noexcept {
  const double discriminant = (double)b * (double)b - 4 * (double)a * (double)c;
  if (discriminant < 0)
    return false;
  const double rootDiscriminant = std::sqrt(discriminant);
  double q;
  if (b < 0)
    q = -.5 * (b - rootDiscriminant);
  else
    q = -.5 * (b + rootDiscriminant);
  *t0 = q / a;
  *t1 = c / q;
  if (*t0 > *t1)
    std::swap(*t0, *t1);
  return true;
}
} // namespace temprenderer::core::math
