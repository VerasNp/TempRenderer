#include "temprenderer/renderer/Camera.hpp"

namespace temprenderer::renderer {
Camera::Camera(std::string name, const kwp::Point3 &eye, float focalLength,
               std::uint16_t resolutionWidth, std::uint16_t resolutionHeight,
               float viewportWidth, float viewportHeight) {
  this->name_ = name;
  this->eye_ = eye;
  this->resolutionWidth_ = resolutionWidth;
  this->resolutionHeight_ = resolutionHeight;
  this->focalLength_ = focalLength;
  this->viewPortPixelDx_ = viewportWidth / resolutionWidth;
  this->viewPortPixelDy_ = viewportHeight / resolutionHeight;
  this->viewPortUpperLeft_ = this->eye_ - kwp::Vec3(0, 0, this->focalLength_) -
                             kwp::Vec3(viewportWidth / 2, 0, 0) +
                             kwp::Vec3(0, viewportHeight / 2, 0);
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
  const kwp::Point3 target{pixelXCenter, pixelYCenter, pixelZCenter};
  return core::math::Ray(this->eye_, (target - this->eye_).normalize());
}
} // namespace temprenderer::renderer
