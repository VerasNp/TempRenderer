#include "renderer/PhongIntegrator.hpp"

#include "temprenderer/core/math/Materials.hpp"

namespace temprenderer::renderer {

core::math::Color PhongIntegrator::li(const core::math::Ray &ray,
                                      const scene::Scene &scene) const {
  scene::SurfaceInteraction isect{};
  if (scene.intersect(ray, &isect)) {
    kwp::Point3 origin;
    kwp::Vec3 w = scene.getLight()->getPosition() - origin;
    w = w.normalize();
  }
  return backgroundColor_;
}

} // namespace temprenderer::renderer
