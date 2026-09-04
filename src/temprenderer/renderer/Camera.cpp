#include "temprenderer/renderer/Camera.hpp"

namespace temprenderer::renderer {
Camera::Camera(const kwp::Point3 &eye, unsigned int resolutionWidth,
               unsigned int resolutionHeight, float viewportWidth,
               float viewportHeight) {
  this->eye_ = eye;
  this->resolutionWidth_ = resolutionWidth;
  this->resolutionHeight_ = resolutionHeight;
  this->viewPortPixelDx_ = viewportWidth / resolutionWidth;
  this->viewPortPixelDy_ = viewportHeight / resolutionHeight;
  this->viewPortUpperLeft_ =
      this->eye_ - kwp::Vec3(0, 0, this->cameraConfig_.focalLength) -
      kwp::Vec3(viewportWidth / 2, 0, 0) + kwp::Vec3(0, viewportHeight / 2, 0);
}

core::math::Ray Camera::generateRay(unsigned int col,
                                    unsigned int row) const noexcept {
  const kwp::Scalar pixelXCenter = this->viewPortUpperLeft_.x +
                                   (this->viewPortPixelDx_ / 2) +
                                   (col * this->viewPortPixelDx_);
  const kwp::Scalar pixelYCenter = this->viewPortUpperLeft_.y -
                                   (this->viewPortPixelDy_ / 2) -
                                   (row * this->viewPortPixelDy_);
  const kwp::Scalar pixelZCenter = -this->cameraConfig_.focalLength;
  const kwp::Point3 target{pixelXCenter, pixelYCenter, pixelZCenter};
  return core::math::Ray(this->eye_, (target - this->eye_).normalize());
}
} // namespace temprenderer::renderer
