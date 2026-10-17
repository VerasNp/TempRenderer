#include "renderer/Cylinder.hpp"

#include "core/math/utils.hpp"
#include "kwp/Matrix22.hpp"
#include "kwp/Matrix33.hpp"

namespace temprenderer::renderer {
Cylinder::Cylinder(const kwp::Point3 &baseCenter, const kwp::Scalar baseRadius,
                   const kwp::Point3 &topCenter,
                   const core::math::Material &material) noexcept {
  this->baseCenter_ = baseCenter;
  this->baseRadius_ = baseRadius;
  this->topRadius_ = baseRadius;
  this->topCenter_ = topCenter;
  this->material_ = material;
  kwp::Vec3 cylinderAxis = this->topCenter_ - this->baseCenter_;
  this->height_ = cylinderAxis.length();
  this->cylinderDirection_ = cylinderAxis.normalize();
  const auto frame = core::math::makeTextureFrame(this->cylinderDirection_);
  this->tangent_ = frame.first;
  this->bitangent_ = frame.second;
}

bool Cylinder::intersect(const core::math::Ray &ray,
                         scene::SurfaceInteraction *isec) const noexcept {
  kwp::Scalar tLateral = -1;
  const kwp::Matrix33 m =
      kwp::identity33() -
      kwp::outerProduct(this->cylinderDirection_, this->cylinderDirection_);
  bool hitLateral = false;
  {
    const kwp::Vec3 v = ray.getOrigin() - this->baseCenter_;
    const kwp::Scalar a = kwp::dot(
        kwp::multiplyRowVector(ray.getDirection(), m), ray.getDirection());
    const kwp::Scalar b =
        2 * kwp::dot(kwp::multiplyRowVector(ray.getDirection(), m), v);
    kwp::Scalar c = kwp::dot(kwp::multiplyRowVector(v, m), v) -
                    std::pow(this->baseRadius_, 2);
    kwp::Scalar t0;
    kwp::Scalar t1;
    if (core::math::quadratic(a, b, c, &t0, &t1)) {
      if (t0 < 0) {
        t0 = t1;
      }
      if (t0 >= 0) {
        const kwp::Point3 pi = ray(t0);
        const kwp::Vec3 wi = pi - this->baseCenter_;
        if (const kwp::Scalar projectedHeight =
                kwp::dot(wi, this->cylinderDirection_);
            projectedHeight >= 0 && projectedHeight <= this->height_) {
          tLateral = t0;
          hitLateral = true;
        }
      }
    }
  }

  kwp::Scalar tBase = -1;
  bool hitBase = false;
  {
    const kwp::Vec3 baseNormal = -this->cylinderDirection_;
    if (const kwp::Scalar denom = kwp::dot(baseNormal, ray.getDirection());
        std::abs(denom) > kwp::epsilon) {
      const kwp::Vec3 v = ray.getOrigin() - this->baseCenter_;
      if (kwp::Scalar tb = (-kwp::dot(v, baseNormal)) / denom; tb >= 0) {
        const kwp::Point3 p = ray(tb);
        if (const kwp::Scalar distFromCenter = (p - this->baseCenter_).length();
            distFromCenter <= this->baseRadius_) {
          tBase = tb;
          hitBase = true;
        }
      }
    }
  }

  kwp::Scalar tTop = -1;
  bool hitTop = false;
  {
    const kwp::Vec3 topNormal = this->cylinderDirection_;
    if (const kwp::Scalar denom = kwp::dot(topNormal, ray.getDirection());
        std::abs(denom) > kwp::epsilon) {
      const kwp::Vec3 v = ray.getOrigin() - this->topCenter_;
      if (const kwp::Scalar tt = (-kwp::dot(v, topNormal)) / denom; tt >= 0) {
        const kwp::Point3 p = ray(tt);
        if (const kwp::Scalar distFromCenter = (p - this->topCenter_).length();
            distFromCenter <= this->topRadius_) {
          tTop = tt;
          hitTop = true;
        }
      }
    }
  }
  if (!hitLateral && !hitBase && !hitTop) {
    return false;
  }
  enum class HitType { kLateral, kBase, kTop };
  kwp::Scalar bestT = std::numeric_limits<kwp::Scalar>::max();
  auto bestType = HitType::kLateral;
  if (hitLateral && tLateral < bestT) {
    bestT = tLateral;
    bestType = HitType::kLateral;
  }
  if (hitBase && tBase < bestT) {
    bestT = tBase;
    bestType = HitType::kBase;
  }
  if (hitTop && tTop < bestT) {
    bestT = tTop;
    bestType = HitType::kTop;
  }
  isec->point = ray(bestT);
  switch (bestType) {
  case HitType::kLateral:
    isec->normal = computeLateralNormal(isec->point);
    break;
  case HitType::kBase:
    isec->normal = -this->cylinderDirection_;
    break;
  case HitType::kTop:
    isec->normal = this->cylinderDirection_;
    break;
  }
  constexpr float pi = 3.14159265358979323846F;
  const kwp::Vec3 offset = isec->point - this->baseCenter_;
  const kwp::Scalar axial = kwp::dot(offset, this->cylinderDirection_);
  const kwp::Vec3 radial = offset - this->cylinderDirection_ * axial;
  isec->textureContext.worldPosition = isec->point;
  isec->textureContext.localPosition =
      kwp::Point3{kwp::dot(offset, this->tangent_), axial,
                  kwp::dot(offset, this->bitangent_)};
  isec->textureContext.normal = isec->normal;
  if (bestType == HitType::kLateral) {
    isec->textureContext.repeatV = false;
    isec->textureContext.uv = {
        0.5F + std::atan2(kwp::dot(radial, this->bitangent_),
                          kwp::dot(radial, this->tangent_)) /
                   (2.0F * pi),
        axial / this->height_};
  } else {
    isec->textureContext.repeatU = false;
    isec->textureContext.repeatV = false;
    const kwp::Point3 capCenter =
        bestType == HitType::kBase ? this->baseCenter_ : this->topCenter_;
    const kwp::Vec3 capOffset = isec->point - capCenter;
    const kwp::Scalar radius =
        bestType == HitType::kBase ? this->baseRadius_ : this->topRadius_;
    isec->textureContext.uv = {
        0.5F + kwp::dot(capOffset, this->tangent_) / (2.0F * radius),
        0.5F + kwp::dot(capOffset, this->bitangent_) / (2.0F * radius)};
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
