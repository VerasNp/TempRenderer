#include "renderer/Triangle.hpp"

#include "kwp/Vec2.hpp"

namespace temprenderer::renderer {
Triangle::Triangle(const kwp::Point3 p1, const kwp::Point3 p2,
                   const kwp::Point3 p3, const core::math::Material &material) {
  this->p1_ = p1;
  this->p2_ = p2;
  this->p3_ = p3;
  this->e1_ = this->p2_ - this->p1_;
  this->e2_ = this->p3_ - this->p1_;
  this->normal_ = kwp::cross(this->e1_, this->e2_).normalize();
  this->material_ = material;
}
bool Triangle::intersect(const core::math::Ray &ray,
                         scene::SurfaceInteraction *isect) const noexcept {
  if (this->cullBackfaces_ &&
      kwp::dot(this->normal_, ray.getDirection()) >= 0) {
    return false;
  }
  kwp::Scalar denom = kwp::dot(this->normal_, ray.getDirection());
  if (std::abs(denom) < kwp::epsilon) {
    return false;
  }
  const kwp::Vec3 w = ray.getOrigin() - this->p1_;
  kwp::Scalar ti = -kwp::dot(this->normal_, w) / denom;
  if (ti < kwp::epsilon) {
    return false;
  }
  kwp::Point3 pi = ray(ti);
  kwp::Scalar areaTotal = kwp::cross(this->e1_, this->e2_).length();
  if (areaTotal < kwp::epsilon) {
    return false;
  }
  kwp::Vec3 r1 = this->p1_ - pi;
  kwp::Vec3 r2 = this->p2_ - pi;
  kwp::Vec3 r3 = this->p3_ - pi;
  kwp::Scalar alpha = (kwp::dot(kwp::cross(r1, r2), this->normal_)) /
                      kwp::length(this->normal_);
  kwp::Scalar beta = (kwp::dot(kwp::cross(r2, r3), this->normal_)) /
                     kwp::length(this->normal_);
  kwp::Scalar gamma = (kwp::dot(kwp::cross(r3, r1), this->normal_)) /
                      kwp::length(this->normal_);
  if (alpha < kwp::epsilon || beta < kwp::epsilon || gamma < kwp::epsilon) {
    return false;
  }
  isect->point = pi;
  isect->normal = this->normal_;
  isect->material = this->material_;
  return true;
}
} // namespace temprenderer::renderer
