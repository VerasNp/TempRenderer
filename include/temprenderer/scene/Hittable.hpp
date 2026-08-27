#pragma once
#include "temprenderer/core/math/Ray.hpp"

namespace temprenderer::scene {

class Hittable {
public:
  virtual ~Hittable() = default;

  [[nodiscard]] virtual bool
  intersect(const core::math::Ray &ray) const noexcept = 0;
};

} // namespace temprenderer::scene
