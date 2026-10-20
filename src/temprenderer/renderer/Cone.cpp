#include "renderer/Cone.hpp"
#include "kwp/Matrix33.hpp"
#include "temprenderer/core/math/utils.hpp"

namespace temprenderer::renderer {
Cone::Cone(const kwp::Point3 &baseCenter, const kwp::Scalar baseRadius,
           const kwp::Scalar height, const kwp::Vec3 &direction,
           const bool hasBottom,
           const core::math::Material &material) noexcept {
  this->baseCenter_ = baseCenter;
  this->baseRadius_ = baseRadius;
  this->material_ = material;
  this->height_ = height;
  this->coneDirection_ = direction.normalize();
  this->hasBottom_ = hasBottom;
  this->vertex_ = this->baseCenter_ + (this->coneDirection_ * this->height_);
}

bool Cone::intersect(const core::math::Ray &ray,
                     scene::SurfaceInteraction *isec) const noexcept {
  enum class HitType { kNone, kLateral, kBase };
  kwp::Scalar bestT = std::numeric_limits<kwp::Scalar>::max();
  auto bestType = HitType::kNone;
  {
    const kwp::Vec3 v = ray.getOrigin() - this->baseCenter_;
    const kwp::Vec3 &d = ray.getDirection();
    const kwp::Matrix33 m =
        kwp::identity33() -
        kwp::outerProduct(this->coneDirection_, this->coneDirection_);

    const kwp::Scalar k2 = (this->baseRadius_ * this->baseRadius_) /
                           (this->height_ * this->height_);
    const kwp::Scalar dd = kwp::dot(d, this->coneDirection_);
    const kwp::Scalar vd = kwp::dot(v, this->coneDirection_);
    const kwp::Vec3 vm = kwp::multiplyRowVector(v, m);

    const kwp::Scalar a =
        kwp::dot(kwp::multiplyRowVector(d, m), d) - k2 * dd * dd;
    const kwp::Scalar b =
        2 * kwp::dot(vm, d) - 2 * k2 * dd * (vd - this->height_);
    const kwp::Scalar c =
        kwp::dot(vm, v) - k2 * (this->height_ - vd) * (this->height_ - vd);

    kwp::Scalar roots[2];
    int rootCount = 0;
    if (std::abs(a) > kwp::epsilon) {
      kwp::Scalar t0;
      kwp::Scalar t1;
      if (core::math::quadratic(a, b, c, &t0, &t1)) {
        roots[rootCount++] = t0;
        roots[rootCount++] = t1;
      }
    } else if (std::abs(b) > kwp::epsilon) {
      roots[rootCount++] = -c / b;
    }
    for (int i = 0; i < rootCount; ++i) {
      const kwp::Scalar t = roots[i];
      if (t < kwp::epsilon) {
        continue;
      }
      const kwp::Scalar h =
          kwp::dot(ray(t) - this->baseCenter_, this->coneDirection_);
      if (h >= 0 && h <= this->height_) {
        bestT = t;
        bestType = HitType::kLateral;
        break;
      }
    }
  }
  if (this->hasBottom_) {
    const kwp::Vec3 baseNormal = -this->coneDirection_;
    const kwp::Scalar denom = kwp::dot(baseNormal, ray.getDirection());
    if (std::abs(denom) > kwp::epsilon) {
      const kwp::Scalar tb =
          kwp::dot(this->baseCenter_ - ray.getOrigin(), baseNormal) / denom;
      if (tb >= kwp::epsilon && tb < bestT &&
          (ray(tb) - this->baseCenter_).length() <= this->baseRadius_) {
        bestT = tb;
        bestType = HitType::kBase;
      }
    }
  }
  if (bestType == HitType::kNone) {
    return false;
  }
  isec->point = ray(bestT);
  if (bestType == HitType::kBase) {
    isec->normal = -this->coneDirection_;
  } else {
    kwp::Vec3 n = computeLateralNormal(isec->point);
    if (!this->hasBottom_ && kwp::dot(n, ray.getDirection()) > 0) {
      n = -n;
    }
    isec->normal = n;
  }
  isec->material = material_;
  return true;
}

kwp::Vec3 Cone::computeLateralNormal(
    const kwp::Point3 &intersectionPoint) const noexcept {
  const kwp::Vec3 k = this->vertex_ - intersectionPoint;
  const kwp::Vec3 w = kwp::cross(k, this->coneDirection_);
  const kwp::Vec3 n = kwp::cross(w, k);
  return n.normalize();
}
} // namespace temprenderer::renderer
