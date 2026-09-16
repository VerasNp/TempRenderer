#include "renderer/Plane.hpp"

namespace temprenderer::renderer {
Plane::Plane(const kwp::Point3 &point, const kwp::Vec3 &normal,
             core::math::Material material) noexcept {
  this->point_ = point;
  this->normal_ = normal;
  this->material_ = material;
}
bool Plane::intersect(const core::math::Ray &ray,
                      scene::SurfaceInteraction *isec) const noexcept {
  const kwp::Scalar w = kwp::dot(ray.getDirection(), this->normal_);
  if (std::abs(w) < kwp::epsilon) {
    return false;
  }
  const kwp::Vec3 v = ray.getOrigin() - this->point_;
  const kwp::Scalar tIntersection = -kwp::dot(this->normal_, v) / w;
  if (tIntersection <= kwp::epsilon) {
    return false;
  }
  isec->point = ray(tIntersection);
  isec->normal = this->normal_;
  isec->material = material_;
  return true;
}
} // namespace temprenderer::renderer
