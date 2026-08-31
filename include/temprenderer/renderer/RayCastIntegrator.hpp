#pragma once
#include "SamplerIntegrator.hpp"

namespace temprenderer::renderer {
/**
* @brief Simplest possible Integrator: resolves each pixel with a single primary ray and a binary hit/miss test, no shading or secondary rays.
 */
class RayCastIntegrator final : public SamplerIntegrator {
public:
  RayCastIntegrator(const Camera &camera, unsigned int width,
                    unsigned int height, core::math::Color hitColor,
                    core::math::Color backgroundColor) noexcept
      : SamplerIntegrator(camera, width, height), hitColor_(hitColor),
        backgroundColor_(backgroundColor) {}

protected:
  /**
   * @brief Resolves the color seen along a single ray.
   *
   * @param ray The primary ray to test, in world space.
   * @param world The scene to test the ray against.
   * @return hitColor_ on any intersection, backgroundColor_ otherwise
   */
  [[nodiscard]] core::math::Color
  li(const core::math::Ray &ray, const scene::Hittable &world) const override;

private:
  core::math::Color hitColor_;
  core::math::Color backgroundColor_;
};

} // namespace temprenderer::renderer
