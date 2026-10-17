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
  const kwp::Vec3 localPosition = isec->point - this->center_;
  const float nx = localPosition.x / this->radius_;
  const float ny = localPosition.y / this->radius_;
  const float nz = localPosition.z / this->radius_;
  constexpr float pi = 3.14159265358979323846F;
  isec->textureContext.worldPosition = isec->point;
  isec->textureContext.localPosition =
      kwp::Point3{localPosition.x, localPosition.y, localPosition.z};
  isec->textureContext.normal = isec->normal;
  isec->textureContext.repeatV = false;
  isec->textureContext.uv = {0.5F + std::atan2(nz, nx) / (2.0F * pi),
                             0.5F -
                                 std::asin(std::clamp(ny, -1.0F, 1.0F)) / pi};
  isec->material = material_;
  return true;
}
