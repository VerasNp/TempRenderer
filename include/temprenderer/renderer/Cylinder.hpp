#pragma once
#include "core/math/Materials.hpp"
#include "temprenderer/scene/Hittable.hpp"

namespace temprenderer::renderer {
/**
* @brief A cylinder primitive, defined by a center of base, radius of base and vertex in world space.
 */
class Cylinder : public scene::Hittable {
public:
  Cylinder(const kwp::Point3 &baseCenter, kwp::Scalar baseRadius,
           const kwp::Point3 &topCenter,
           const core::math::Material &material) noexcept;

  /**
   * @brief Tests whether a ray intersects this cylinder
   *
   * @param ray The ray to test, in world space.
   * @param isec Information about the intersection local.
  * @return true if the ray intersects the cylinder at a non-negative parameter t, false otherwise (including when the ray misses entirely or intersects only behind its origin).
   */
  [[nodiscard]] bool
  intersect(const core::math::Ray &ray,
            scene::SurfaceInteraction *isec) const noexcept override;

  kwp::Vec3 computeLateralNormal(const kwp::Point3 &point) const noexcept;

private:
  kwp::Point3 baseCenter_;
  kwp::Point3 topCenter_;
  kwp::Scalar baseRadius_;
  kwp::Scalar topRadius_;
  kwp::Scalar height_;
  kwp::Vec3 cylinderDirection_;
  core::math::Material material_;
};

} // namespace temprenderer::renderer
