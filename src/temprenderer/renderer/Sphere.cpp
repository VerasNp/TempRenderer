#include "temprenderer/renderer/Sphere.hpp"
#include "temprenderer/core/math/utils.hpp"

namespace temprenderer::scene {
Sphere::Sphere(const kwp::Point3 &center, const kwp::Scalar radius,
               const core::math::Material &material) noexcept {
  this->radius_ = radius;
  this->center_ = center;
  this->material_ = material;
}

bool Sphere::intersect(const core::math::Ray &ray,
                       SurfaceInteraction *isec) const noexcept {
  enum class HitType { kNone, kSurface };
  kwp::Scalar bestTHit = std::numeric_limits<kwp::Scalar>::max();
  auto hitType = HitType::kNone;
  const kwp::Vec3 v = (ray.getOrigin() - this->center_);
  const kwp::Scalar b = 2 * (kwp::dot(v, ray.getDirection()));
  const kwp::Scalar c = kwp::dot(v, v) - (this->radius_ * this->radius_);
  kwp::Scalar t0;
  kwp::Scalar t1;
  if (core::math::quadratic(1, b, c, &t0, &t1)) {
    for (const kwp::Scalar t : {t0, t1}) {
      if (t < kwp::epsilon) {
        continue;
      }
      bestTHit = t;
      hitType = HitType::kSurface;
    }
  }
  if (hitType == HitType::kNone) {
    return false;
  }
  isec->point = ray(bestTHit);
  isec->normal = (-(this->center_ - isec->point)).normalize();
  isec->material = material_;
  return true;
}
} // namespace temprenderer::scene
