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
  const auto frame = core::math::makeTextureFrame(this->coneDirection_);
  this->tangent_ = frame.first;
  this->bitangent_ = frame.second;
}

bool Cone::intersect(const core::math::Ray &ray,
                     scene::SurfaceInteraction *isec) const noexcept {
  kwp::Vec3 v = ray.getOrigin() - this->baseCenter_;
  kwp::Scalar tLateral = -1;
  bool hitLateral = false;
  {
    kwp::Matrix33 M =
        kwp::identity33() -
        kwp::outerProduct(this->coneDirection_, this->coneDirection_);
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
      if (t0 < 0) {
        t0 = t1;
      }
      if (t0 >= 0) {
        const kwp::Point3 pi = ray(t0);
        const kwp::Vec3 wi = pi - this->baseCenter_;
        const kwp::Scalar projectedHeight = kwp::dot(wi, this->coneDirection_);
        if (projectedHeight >= 0 && projectedHeight <= this->height_) {
          tLateral = t0;
          hitLateral = true;
        }
      }
    }
  }

  kwp::Scalar tBase = -1;
  bool hitBase = false;
  {
    kwp::Vec3 coneBaseNormal = -this->coneDirection_;
    kwp::Scalar denom = kwp::dot(coneBaseNormal, ray.getDirection());
    if (std::abs(denom) > 1e-6F) {
      kwp::Scalar tb =
          kwp::dot(this->baseCenter_ - ray.getOrigin(), coneBaseNormal) / denom;
      if (tb >= 0) {
        kwp::Point3 p = ray(tb);
        kwp::Scalar distFromCenter = (p - this->baseCenter_).length();
        if (distFromCenter <= this->baseRadius_) {
          tBase = tb;
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
  constexpr float pi = 3.14159265358979323846F;
  const kwp::Vec3 offset = isec->point - this->baseCenter_;
  const kwp::Scalar axial = kwp::dot(offset, this->coneDirection_);
  const kwp::Vec3 radial = offset - this->coneDirection_ * axial;
  isec->textureContext.worldPosition = isec->point;
  isec->textureContext.localPosition =
      kwp::Point3{kwp::dot(offset, this->tangent_), axial,
                  kwp::dot(offset, this->bitangent_)};
  isec->textureContext.normal = isec->normal;
  if (isBaseHit) {
    isec->textureContext.repeatU = false;
    isec->textureContext.repeatV = false;
    isec->textureContext.uv = {
        0.5F + kwp::dot(radial, this->tangent_) / (2.0F * this->baseRadius_),
        0.5F + kwp::dot(radial, this->bitangent_) / (2.0F * this->baseRadius_)};
  } else {
    isec->textureContext.repeatV = false;
    isec->textureContext.uv = {
        0.5F + std::atan2(kwp::dot(radial, this->bitangent_),
                          kwp::dot(radial, this->tangent_)) /
                   (2.0F * pi),
        axial / this->height_};
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
