#pragma once
#include "scene/Hittable.hpp"

namespace temprenderer::renderer {
/**
 * @brief TODO
 */
class Cone : public scene::Hittable {
public:
  Cone(const kwp::Point3 &baseCenter, float baseRadius,
       const kwp::Point3 &vertex, core::math::Material material) noexcept;

  /**
   * @brief TODO
   *
   * @param ray
   * @param isect
   * @return
   */
  [[nodiscard]] bool
  intersect(const core::math::Ray &ray,
            scene::SurfaceInteraction *isect) const noexcept override;

private:
  kwp::Point3 baseCenter_;
  float baseRadius_;
  kwp::Point3 vertex_;
  core::math::Material material_;
  kwp::Vec3 direction_;
  float height_;
};
} // namespace temprenderer::renderer
