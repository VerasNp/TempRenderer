#pragma once
#include "SamplerIntegrator.hpp"
#include "scene/Scene.hpp"

namespace temprenderer::renderer {
/**
 * @brief TODO
 */
class PhongIntegrator : public SamplerIntegrator {
public:
  PhongIntegrator(const Camera &camera, unsigned int width, unsigned int height,
                  core::math::Color backgroundColor) noexcept
      : SamplerIntegrator(camera, width, height),
        backgroundColor_(backgroundColor) {}

protected:
  /**
   * @brief Resolves the color seen along a single ray.
   *
   * @param ray The primary ray to test, in world space.
   * @param scene The scene to test the ray against.
   * @return hitColor_ on any intersection, backgroundColor_ otherwise
   */
  [[nodiscard]] core::math::Color li(const core::math::Ray &ray,
                                     const scene::Scene &scene) const override;

private:
  core::math::Color backgroundColor_;
};

} // namespace temprenderer::renderer
