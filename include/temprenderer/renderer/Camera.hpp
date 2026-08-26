#pragma once
#include "kwp/Point3.hpp"

namespace temprenderer::renderer {
class Camera {
public:
  Camera(const kwp::Point3 &eye, unsigned int resolutionWidth,
         unsigned int resolutionHeight, unsigned int imageWidth,
         unsigned int imageHeight);

private:
  kwp::Point3 eye_;
};
} // namespace temprenderer::renderer
