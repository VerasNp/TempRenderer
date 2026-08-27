#pragma once
#include "SampleIntegrator.hpp"

namespace temprenderer::renderer {

class RayCastIntegrator final : public SamplerIntegrator {
public:
  RayCastIntegrator(const Camera &camera, unsigned int width,
                    unsigned int height, core::math::Color hitColor,
                    core::math::Color backgroundColor) noexcept
      : SamplerIntegrator(camera, width, height), hitColor_(hitColor),
        backgroundColor_(backgroundColor) {}

protected:
  [[nodiscard]] core::math::Color
  Li(const core::math::Ray &ray, const scene::Hittable &world) const override;

private:
  core::math::Color hitColor_;
  core::math::Color backgroundColor_;
};

} // namespace temprenderer::renderer
