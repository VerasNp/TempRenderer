#include "temprenderer/renderer/RayCastIntegrator.hpp"
#include "temprenderer/core/math/Color.hpp"

namespace temprenderer::renderer {

core::math::Color RayCastIntegrator::li(const core::math::Ray &ray,
                                        const scene::Hittable &world) const {
  return world.intersect(ray) ? hitColor_ : backgroundColor_;
}

} // namespace temprenderer::renderer
