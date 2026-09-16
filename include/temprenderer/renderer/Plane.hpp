#pragma once
#include "core/math/Materials.hpp"
#include "temprenderer/scene/Hittable.hpp"

namespace temprenderer::renderer {
/**
* @brief A plane primitive, defined by a center and radius in world space.
 */
class Plane : public scene::Hittable {
public:
  Plane(const kwp::Point3 &point, const kwp::Vec3 &normal,
        core::math::Material material) noexcept;

  /**
   * @brief Tests whether a ray intersects this plane
   *
   * @param ray The ray to test, in world space.
   * @param isec Information about the intersection local.
  * @return true if the ray intersects the plane at a non-negative parameter t, false otherwise (including when the ray misses entirely or intersects only behind its origin).
   */
  [[nodiscard]] bool
  intersect(const core::math::Ray &ray,
            scene::SurfaceInteraction *isec) const noexcept override;

private:
  kwp::Point3 point_;
  kwp::Vec3 normal_;
  core::math::Material material_;
};

} // namespace temprenderer::renderer
