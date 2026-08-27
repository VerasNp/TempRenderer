#pragma once
#include "temprenderer/core/math/Color.hpp"
#include "temprenderer/renderer/Camera.hpp"
#include "temprenderer/renderer/Integrator.hpp"

namespace temprenderer::renderer {

class SamplerIntegrator : public Integrator {
public:
  SamplerIntegrator(const Camera &camera, unsigned int width,
                    unsigned int height) noexcept
      : camera_(camera), width_(width), height_(height) {}

  [[nodiscard]] Canvas render(const scene::Hittable &world) final {
    Canvas canvas(width_, height_);
    for (unsigned int row = 0; row < height_; ++row) {
      for (unsigned int col = 0; col < width_; ++col) {
        const core::math::Ray ray = camera_.generateRay(col, row);
        canvas.set(col, row, Li(ray, world));
      }
    }

    return canvas;
  }

protected:
  [[nodiscard]] virtual core::math::Color
  Li(const core::math::Ray &ray, const scene::Hittable &world) const = 0;

  const Camera &camera_;
  unsigned int width_, height_;
};

} // namespace temprenderer::renderer
