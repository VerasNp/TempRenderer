#pragma once
#include "core/math/Materials.hpp"
#include "temprenderer/scene/Hittable.hpp"

#include <memory>

namespace temprenderer::scene {
/**
* @brief A sphere primitive, defined by a center and radius in world space.
 */
class Sphere : public Hittable {
public:
  Sphere(const kwp::Point3 &center, kwp::Scalar radius,
         std::shared_ptr<core::math::Material> color) noexcept;

  /**
   * @brief Tests whether a ray intersects this sphere
   *
   * @param ray The ray to test, in world space.
   * @param isec Information about the intersection local.
  * @return true if the ray intersects the sphere at a non-negative parameter t, false otherwise (including when the ray misses entirely or intersects only behind its origin).
   */
  [[nodiscard]] bool
  intersect(const core::math::Ray &ray,
            SurfaceInteraction *isec) const noexcept override;

private:
  kwp::Point3 center_;
  kwp::Scalar radius_;
  std::shared_ptr<core::math::Material> material_;
};

} // namespace temprenderer::scene
