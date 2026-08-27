#pragma once
#include "temprenderer/scene/Hittable.hpp"
#include <kwp/Vec3.hpp>

namespace temprenderer::scene {

class Sphere : public Hittable {
public:
  Sphere(const kwp::Point3 &center, kwp::Scalar radius) noexcept;

  [[nodiscard]] bool
  intersect(const core::math::Ray &ray) const noexcept override;

private:
  kwp::Point3 center_;
  kwp::Scalar radius_;
};

} // namespace temprenderer::scene
