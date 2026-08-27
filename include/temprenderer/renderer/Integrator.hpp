#pragma once
#include "temprenderer/renderer/Canvas.hpp"
#include "temprenderer/scene/Hittable.hpp"

namespace temprenderer::renderer {

class Integrator {
public:
  virtual ~Integrator() = default;

  [[nodiscard]] virtual Canvas render(const scene::Hittable &world) = 0;
};

} // namespace temprenderer::renderer
