#include "temprenderer/renderer/Camera.hpp"

#include <cassert>
#include <cmath>

namespace temprenderer::renderer {
Camera::Camera(const std::string &name, const kwp::Point3 &eye,
               const kwp::Point3 &lookAt, const float focalLength,
               const std::uint16_t resolutionWidth,
               const std::uint16_t resolutionHeight, const float viewportWidth,
               const float viewportHeight) {
  constexpr kwp::Vec3 worldUp{0.0F, 1.0F, 0.0F};
  this->name_ = name;
  this->eye_ = eye;
  this->lookAt_ = lookAt;
  const kwp::Vec3 toTarget = this->lookAt_ - this->eye_;
  const kwp::Vec3 forward = toTarget.normalize();
  const kwp::Vec3 right = kwp::cross(forward, worldUp).normalize();
  const kwp::Vec3 camUp = kwp::cross(right, forward);
  this->cameraToWorld_ = kwp::Matrix33{
      right.x,    camUp.x, -forward.x, right.y,    camUp.y,
      -forward.y, right.z, camUp.z,    -forward.z,
  };
  this->resolutionWidth_ = resolutionWidth;
  this->resolutionHeight_ = resolutionHeight;
  this->focalLength_ = focalLength;
  this->viewPortPixelDx_ = viewportWidth / resolutionWidth;
  this->viewPortPixelDy_ = viewportHeight / resolutionHeight;
  this->viewPortUpperLeft_ =
      kwp::Point3{-viewportWidth / 2, viewportHeight / 2, -this->focalLength_};
}

core::math::Ray Camera::generateRay(unsigned int col,
                                    unsigned int row) const noexcept {
  const kwp::Scalar pixelXCenter = this->viewPortUpperLeft_.x +
                                   (this->viewPortPixelDx_ / 2) +
                                   (col * this->viewPortPixelDx_);
  const kwp::Scalar pixelYCenter = this->viewPortUpperLeft_.y -
                                   (this->viewPortPixelDy_ / 2) -
                                   (row * this->viewPortPixelDy_);
  const kwp::Scalar pixelZCenter = -this->focalLength_;
  const kwp::Vec3 local{pixelXCenter, pixelYCenter, pixelZCenter};
  return core::math::Ray(this->eye_,
                         (this->cameraToWorld_ * local).normalize());
}
} // namespace temprenderer::renderer
