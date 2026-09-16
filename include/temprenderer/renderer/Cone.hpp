#pragma once
#include "core/math/Materials.hpp"
#include "temprenderer/scene/Hittable.hpp"

namespace temprenderer::renderer {
/**
* @brief A cone primitive, defined by a center of base, radius of base and vertex in world space.
 */
class Cone : public scene::Hittable {
public:
  Cone(const kwp::Point3 &baseCenter, kwp::Scalar baseRadius,
       const kwp::Point3 &vertex, core::math::Material material) noexcept;

  /**
   * @brief Tests whether a ray intersects this cone
   *
   * @param ray The ray to test, in world space.
   * @param isec Information about the intersection local.
  * @return true if the ray intersects the cone at a non-negative parameter t, false otherwise (including when the ray misses entirely or intersects only behind its origin).
   */
  [[nodiscard]] bool
  intersect(const core::math::Ray &ray,
            scene::SurfaceInteraction *isec) const noexcept override;

  kwp::Vec3 computeLateralNormal(const kwp::Point3 &point) const noexcept;

private:
  kwp::Point3 baseCenter_;
  kwp::Scalar baseRadius_;
  kwp::Scalar height_;
  kwp::Point3 vertex_;
  kwp::Vec3 coneDirection_;
  core::math::Material material_;
};

} // namespace temprenderer::renderer
