#include "temprenderer/renderer/Sphere.hpp"
#include "temprenderer/core/math/utils.hpp"

#include <memory>
namespace temprenderer::renderer {} // namespace temprenderer::renderer
temprenderer::scene::Sphere::Sphere(const kwp::Point3 &center,
                                    kwp::Scalar radius,
                                    core::math::Material material) noexcept {
  this->radius_ = radius;
  this->center_ = center;
  this->material_ = material;
}
bool temprenderer::scene::Sphere::intersect(
    const core::math::Ray &ray, SurfaceInteraction *isec) const noexcept {
  const kwp::Vec3 w = (ray.getOrigin() - this->center_);
  kwp::Scalar a = 1;
  kwp::Scalar b = 2 * (dot(w, ray.getDirection()));
  kwp::Scalar c = dot(w, w) - (static_cast<kwp::Scalar>(this->radius_) *
                               static_cast<kwp::Scalar>(this->radius_));
  kwp::Scalar t0;
  kwp::Scalar t1;
  if (!core::math::quadratic(a, b, c, &t0, &t1)) {
    return false;
  }
  if (t0 < 0) {
    t0 = t1;
    if (t0 < 0) {
      return false;
    }
  }
  isec->point = ray(t0);
  isec->normal = (-(this->center_ - isec->point)).normalize();
  isec->material = material_;
  return true;
}
