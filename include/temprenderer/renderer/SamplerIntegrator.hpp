#pragma once
#include "core/math/Materials.hpp"
#include "temprenderer/renderer/Camera.hpp"
#include "temprenderer/renderer/Integrator.hpp"
#include "temprenderer/scene/Scene.hpp"

namespace temprenderer::renderer {
/**
 * @brief Base Integrator
 */
class SamplerIntegrator : public Integrator {
public:
  SamplerIntegrator(const Camera &camera, unsigned int width,
                    unsigned int height) noexcept
      : camera_(camera), width_(width), height_(height) {}

  /**
  * @brief Renders the scene by casting exactly one primary ray per pixel.
   *
   * @param scene The scene to render against.
   * @return A Canvas with every pixel resolved
   */
  [[nodiscard]] Canvas render(const scene::Scene &scene) final {
    Canvas canvas(width_, height_);
    for (unsigned int row = 0; row < height_; ++row) {
      for (unsigned int col = 0; col < width_; ++col) {
        const core::math::Ray ray = camera_.generateRay(col, row);
        canvas.set(col, row, li(ray, scene));
      }
    }
    return canvas;
  }

protected:
  /**
   * @brief Computes the radiance/color seen along a single ray.
   *
  * @param ray The primary ray to evaluate, in world space.
   * @param scene The scene to test/shade against.
   * @return The resolved color for this ray.
   */
  [[nodiscard]] virtual core::math::Color
  li(const core::math::Ray &ray, const scene::Scene &scene) const = 0;

  const Camera &camera_;
  unsigned int width_, height_;
};

} // namespace temprenderer::renderer
