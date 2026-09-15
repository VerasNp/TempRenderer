#include "renderer/Cone.hpp"
#include "kwp/Matrix33.hpp"
#include "temprenderer/core/math/utils.hpp"

namespace temprenderer::renderer {} // namespace temprenderer::renderer
temprenderer::scene::Cone::Cone(const kwp::Point3 &baseCenter,
                                kwp::Scalar baseRadius,
                                const kwp::Point3 &vertex,
                                core::math::Material material) noexcept {
  this->baseCenter_ = baseCenter;
  this->baseRadius_ = baseRadius;
  this->material_ = material;
  const kwp::Vec3 coneAxis = this->vertex_ - this->baseCenter_;
  this->height_ = (coneAxis).length();
  this->coneDirection_ = coneAxis.normalize();
}

bool temprenderer::scene::Cone::intersect(
    const core::math::Ray &ray, SurfaceInteraction *isec) const noexcept {
  kwp::Vec3 v = ray.getOrigin() - this->baseCenter_;
  kwp::Scalar a =
      kwp::dot(
          kwp::multiplyRowVector(ray.getDirection(),
                                 kwp::Matrix33{0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
                                               0.0F, 0.0F, 0.0F, 1.0F}),
          ray.getDirection()) -
      ((std::pow(this->baseRadius_, 2) / std::pow(this->height_, 2)) *
       kwp::dot(ray.getDirection(), this->coneDirection_));
  kwp::Scalar b =
      2 * kwp::dot(kwp::multiplyRowVector(v, kwp::Matrix33{0.0F, 0.0F, 0.0F,
                                                           0.0F, 0.0F, 0.0F,
                                                           0.0F, 0.0F, 1.0F}),
                   ray.getDirection()) -
      (2 * (std::pow(this->baseRadius_, 2) / std::pow(this->height_, 2)) *
       kwp::dot(ray.getDirection(), this->coneDirection_) *
       (-this->height_ + (kwp::dot(v, this->coneDirection_))));
  kwp::Scalar c =
      kwp::dot(
          kwp::multiplyRowVector(v, kwp::Matrix33{0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
                                                  0.0F, 0.0F, 0.0F, 1.0F}),
          v) -
      ((std::pow(this->baseRadius_, 2) / std::pow(this->height_, 2)) *
       (std::pow(this->height_, 2) +
        std::pow(kwp::dot(v, this->coneDirection_), 2) -
        (2 * this->height_ * kwp::dot(v, this->coneDirection_))));
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
  // isec->normal = .normalize();
  isec->material = material_;
  return true;
}
