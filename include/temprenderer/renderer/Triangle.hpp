#pragma once
#include "scene/Hittable.hpp"

namespace temprenderer::renderer {
class Triangle : public scene::Hittable {
public:
  Triangle(kwp::Point3 p1, kwp::Point3 p2, kwp::Point3 p3,
           const core::math::Material &material);

  [[nodiscard]] bool
  intersect(const core::math::Ray &ray,
            scene::SurfaceInteraction *isect) const noexcept override;

private:
  kwp::Point3 p1_;
  kwp::Point3 p2_;
  kwp::Point3 p3_;
  kwp::Vec3 e1_;
  kwp::Vec3 e2_;
  kwp::Vec3 normal_;
  core::math::Material material_;
};
} // namespace temprenderer::renderer
