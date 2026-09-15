#include "renderer/Cone.hpp"
#include "kwp/Matrix33.hpp"
#include "temprenderer/core/math/utils.hpp"

namespace temprenderer::renderer {
Cone::Cone(const kwp::Point3 &baseCenter, kwp::Scalar baseRadius,
           const kwp::Point3 &vertex, core::math::Material material) noexcept {
  this->baseCenter_ = baseCenter;
  this->baseRadius_ = baseRadius;
  this->material_ = material;
  this->vertex_ = vertex;
  const kwp::Vec3 coneAxis = this->vertex_ - this->baseCenter_;
  this->height_ = coneAxis.length();
  this->coneDirection_ = coneAxis.normalize();
}

bool Cone::intersect(const core::math::Ray &ray,
                     scene::SurfaceInteraction *isec) const noexcept {
  kwp::Scalar tLateral = -1;
  bool hitLateral = false;
  {
    kwp::Matrix33 M =
        kwp::identity33() -
        kwp::outerProduct(this->coneDirection_, this->coneDirection_);
    kwp::Vec3 v = ray.getOrigin() - this->baseCenter_;
    kwp::Scalar a =
        kwp::dot(kwp::multiplyRowVector(ray.getDirection(), M),
                 ray.getDirection()) -
        ((std::pow(this->baseRadius_, 2) / std::pow(this->height_, 2)) *
         std::pow(kwp::dot(ray.getDirection(), this->coneDirection_), 2));
    kwp::Scalar b =
        (2 * kwp::dot(kwp::multiplyRowVector(v, M), ray.getDirection())) -
        (2 * (std::pow(this->baseRadius_, 2) / std::pow(this->height_, 2)) *
         kwp::dot(ray.getDirection(), this->coneDirection_) *
         (-this->height_ + (kwp::dot(v, this->coneDirection_))));
    kwp::Scalar c =
        kwp::dot(kwp::multiplyRowVector(v, M), v) -
        ((std::pow(this->baseRadius_, 2) / std::pow(this->height_, 2)) *
         (std::pow(this->height_, 2) +
          std::pow(kwp::dot(v, this->coneDirection_), 2) -
          (2 * this->height_ * kwp::dot(v, this->coneDirection_))));
    kwp::Scalar t0;
    kwp::Scalar t1;
    if (core::math::quadratic(a, b, c, &t0, &t1)) {
      for (kwp::Scalar t : {t0, t1}) {
        if (t < 0)
          continue;
        kwp::Point3 p = ray(t);
        kwp::Vec3 wi = this->baseCenter_ - p; // era: baseCenter_ - p
        kwp::Scalar projectedHeight = kwp::dot(wi, this->coneDirection_);
        if (projectedHeight >= 0 && projectedHeight <= this->height_) {
          tLateral = t;
          hitLateral = true;
          break;
        }
      }
    }
  }

  kwp::Scalar tBase = -1;
  bool hitBase = false;
  {
    kwp::Vec3 planeNormal = -this->coneDirection_;
    kwp::Scalar denom = kwp::dot(planeNormal, ray.getDirection());
    if (std::abs(denom) > 1e-6F) {
      kwp::Scalar t =
          kwp::dot(this->baseCenter_ - ray.getOrigin(), planeNormal) / denom;
      if (t >= 0) {
        kwp::Point3 p = ray(t);
        kwp::Scalar distFromCenter = (p - this->baseCenter_).length();
        if (distFromCenter <= this->baseRadius_) {
          tBase = t;
          hitBase = true;
        }
      }
    }
  }

  if (!hitLateral && !hitBase) {
    return false;
  }

  kwp::Scalar tHit;
  bool isBaseHit;
  if (hitLateral && hitBase) {
    isBaseHit = tBase < tLateral;
    tHit = isBaseHit ? tBase : tLateral;
  } else if (hitLateral) {
    isBaseHit = false;
    tHit = tLateral;
  } else {
    isBaseHit = true;
    tHit = tBase;
  }

  isec->point = ray(tHit);
  isec->normal =
      isBaseHit ? -this->coneDirection_ : computeLateralNormal(isec->point);
  isec->material = material_;
  return true;
  return true;
}

kwp::Vec3 Cone::computeLateralNormal(const kwp::Point3 &point) const noexcept {
  kwp::Vec3 pc = point - this->baseCenter_;
  kwp::Scalar projLength = kwp::dot(pc, this->coneDirection_);
  kwp::Point3 pointOnAxis =
      this->baseCenter_ + this->coneDirection_ * projLength;
  kwp::Vec3 radial = (point - pointOnAxis).normalize();
  kwp::Scalar k = this->baseRadius_ / this->height_;
  return (radial - this->coneDirection_ * k).normalize();
}
} // namespace temprenderer::renderer
