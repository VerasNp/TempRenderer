#pragma once
#include "temprenderer/core/math/Ray.hpp"

namespace temprenderer::scene {
/**
 * @brief Interface for anything a ray can be tested against.
 */
class Hittable {
public:
  virtual ~Hittable() = default;

  /**
   *  @brief Tests whether the given ray intersects this object.
   *
   * @param ray The ray to test, in world space.
   * @return true if the ray intersects this object, false otherwise.
   */
  [[nodiscard]] virtual bool
  intersect(const core::math::Ray &ray) const noexcept = 0;
};

} // namespace temprenderer::scene
