#pragma once
#include "temprenderer/scene/Hittable.hpp"

namespace temprenderer::scene {
/**
* @brief A sphere primitive, defined by a center and radius in world space.
 */
class Sphere : public Hittable {
public:
  Sphere(const kwp::Point3 &center, kwp::Scalar radius) noexcept;

  /**
   * @brief Tests whether a ray intersects this sphere
   *
   * @param ray The ray to test, in world space.
  * @return true if the ray intersects the sphere at a non-negative parameter t, false otherwise (including when the ray misses entirely or intersects only behind its origin).
   */
  [[nodiscard]] bool
  intersect(const core::math::Ray &ray) const noexcept override;

private:
  kwp::Point3 center_;
  kwp::Scalar radius_;
};

} // namespace temprenderer::scene
