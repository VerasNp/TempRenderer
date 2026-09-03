#include "temprenderer/renderer/RayCastIntegrator.hpp"
#include "temprenderer/core/math/Materials.hpp"

namespace temprenderer::renderer {

core::math::Color RayCastIntegrator::li(const core::math::Ray &ray,
                                        const scene::Scene &scene) const {
  scene::SurfaceInteraction isect{};
  if (scene.intersect(ray, &isect)) {
    return isect.material->getColor();
  }
  return backgroundColor_;
}

} // namespace temprenderer::renderer
