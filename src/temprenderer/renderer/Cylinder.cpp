#include "renderer/Cylinder.hpp"

#include "core/math/utils.hpp"
#include "kwp/Matrix22.hpp"
#include "kwp/Matrix33.hpp"

namespace temprenderer::renderer {
Cylinder::Cylinder(const kwp::Point3 &baseCenter, const kwp::Scalar baseRadius,
                   const kwp::Scalar height, const kwp::Vec3 direction,
                   const bool hasTop, const bool hasBottom,
                   const core::math::Material &material) noexcept {
  this->baseCenter_ = baseCenter;
  this->baseRadius_ = baseRadius;
  this->topRadius_ = baseRadius;
  this->height_ = height;
  this->cylinderDirection_ = direction.normalize();
  this->hasTop_ = hasTop;
  this->hasBottom_ = hasBottom;
  this->topCenter_ =
      this->baseCenter_ + (this->cylinderDirection_ * this->height_);
  this->material_ = material;
}

bool Cylinder::intersect(const core::math::Ray &ray,
                         scene::SurfaceInteraction *isec) const noexcept {
  enum class HitType { kNone, kLateral, kBase, kTop };
  kwp::Scalar bestT = std::numeric_limits<kwp::Scalar>::max();
  auto bestType = HitType::kNone;
  {
    const kwp::Matrix33 m =
        kwp::identity33() -
        kwp::outerProduct(this->cylinderDirection_, this->cylinderDirection_);
    const kwp::Vec3 v = ray.getOrigin() - this->baseCenter_;
    const kwp::Vec3 md = kwp::multiplyRowVector(ray.getDirection(), m);

    const kwp::Scalar a = kwp::dot(md, ray.getDirection());
    const kwp::Scalar b = 2 * kwp::dot(md, v);
    const kwp::Scalar c = kwp::dot(kwp::multiplyRowVector(v, m), v) -
                          (this->baseRadius_ * this->baseRadius_);
    kwp::Scalar t0;
    kwp::Scalar t1;
    if (a > kwp::epsilon && core::math::quadratic(a, b, c, &t0, &t1)) {
      for (const kwp::Scalar t : {t0, t1}) {
        if (t < kwp::epsilon) {
          continue;
        }
        const kwp::Vec3 wi = ray(t) - this->baseCenter_;
        const kwp::Scalar h = kwp::dot(wi, this->cylinderDirection_);
        if (h >= 0 && h <= this->height_) {
          bestT = t;
          bestType = HitType::kLateral;
          break;
        }
      }
    }
  }
  auto hitCap = [&](const kwp::Point3 &center, const kwp::Vec3 &normal,
                    const kwp::Scalar radius) -> kwp::Scalar {
    const kwp::Scalar denom = kwp::dot(normal, ray.getDirection());
    if (std::abs(denom) <= kwp::epsilon) {
      return -1;
    }
    const kwp::Scalar t = -kwp::dot(ray.getOrigin() - center, normal) / denom;
    if (t < kwp::epsilon || t >= bestT) {
      return -1;
    }
    if ((ray(t) - center).length() > radius) {
      return -1;
    }
    return t;
  };
  if (this->hasBottom_) {
    const kwp::Scalar t =
        hitCap(this->baseCenter_, -this->cylinderDirection_, this->baseRadius_);
    if (t > 0) {
      bestT = t;
      bestType = HitType::kBase;
    }
  }
  if (this->hasTop_) {
    const kwp::Scalar t =
        hitCap(this->topCenter_, this->cylinderDirection_, this->topRadius_);
    if (t > 0) {
      bestT = t;
      bestType = HitType::kTop;
    }
  }
  if (bestType == HitType::kNone) {
    return false;
  }
  isec->point = ray(bestT);
  switch (bestType) {
  case HitType::kLateral: {
    kwp::Vec3 n = computeLateralNormal(isec->point);
    if (!(this->hasBottom_ && this->hasTop_) &&
        kwp::dot(n, ray.getDirection()) > 0) {
      n = -n;
    }
    isec->normal = n;
    break;
  }
  case HitType::kBase:
    isec->normal = -this->cylinderDirection_;
    break;
  case HitType::kTop:
    isec->normal = this->cylinderDirection_;
    break;
  case HitType::kNone:
    break;
  }
  isec->material = material_;
  return true;
}

kwp::Vec3
Cylinder::computeLateralNormal(const kwp::Point3 &point) const noexcept {
  const kwp::Matrix33 m =
      kwp::identity33() -
      kwp::outerProduct(this->cylinderDirection_, this->cylinderDirection_);
  const kwp::Vec3 n = m * (point - this->baseCenter_);
  return n.normalize();
}

} // namespace temprenderer::renderer
