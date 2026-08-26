#include "temprenderer/renderer/Camera.hpp"
namespace temprenderer::renderer {
Camera::Camera(const kwp::Point3 &eye, unsigned int resolutionWidth,
               unsigned int resolutionHeight, unsigned int imageWidth,
               unsigned int imageHeight) {
  this->viewPortDx_ = imageWidth / resolutionWidth;
  this->viewPortDy_ = imageWidth / resolutionWidth;
}
} // namespace temprenderer::renderer
