#pragma once
#include "core/math/Materials.hpp"
#include "temprenderer/renderer/Camera.hpp"
#include "temprenderer/renderer/Integrator.hpp"
#include "temprenderer/scene/Scene.hpp"
#include "temprenderer/renderer/SamplerRandom.hpp"

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
   * @brief Renders the scene using supersampling.
   *
   * @param scene The scene to render against.
   * @return A Canvas with every pixel resolved
   */
  [[nodiscard]] Canvas render(const scene::Scene &scene) final {
    Canvas canvas(width_, height_);
    constexpr unsigned int samples = 4;
    for (unsigned int row = 0; row < height_; ++row) {
      for (unsigned int col = 0; col < width_; ++col) {
        core::math::ColorF pixel_color{0.0f, 0.0f, 0.0f};
        for (unsigned int s = 0; s < samples; ++s) {
          const core::math::Ray ray = camera_.generateRay(
              col + random_double(),
              row + random_double());
          pixel_color += li(ray, scene).toFloat();
        }
        pixel_color = pixel_color * (1.0f / samples);
        canvas.set(col, row, core::math::Color::fromFloat(pixel_color));
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
