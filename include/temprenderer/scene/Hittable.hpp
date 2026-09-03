#pragma once
#include "core/math/Materials.hpp"
#include "temprenderer/core/math/Ray.hpp"

namespace temprenderer::scene {

struct SurfaceInteraction {
  kwp::Point3 point;
  kwp::Vec3 normal;
  const core::math::Material *material = nullptr;
};

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
   * @param isect Information about the intersection local.
   * @return true if the ray intersects this object, false otherwise.
   */
  [[nodiscard]] virtual bool
  intersect(const core::math::Ray &ray,
            SurfaceInteraction *isect) const noexcept = 0;
};

} // namespace temprenderer::scene
