#include "renderer/PhongIntegrator.hpp"
#include "temprenderer/core/math/Materials.hpp"

#include <algorithm>

namespace temprenderer::renderer {

core::math::Color PhongIntegrator::li(const core::math::Ray &ray,
                                      const scene::Scene &scene) const {
  scene::SurfaceInteraction isect{};
  if (scene.intersect(ray, &isect)) {
    kwp::Vec3 w = (scene.getSpecificLightByType(core::config::LightType::POINT)
                       ->getPosition() -
                   isect.point)
                      .normalize();
    kwp::Vec3 v = (ray.getOrigin() - isect.point).normalize();
    kwp::Vec3 r = (2 * dot(w, isect.normal)) * isect.normal - w;
    core::math::ColorF pointLightColor =
        scene.getSpecificLightByType(core::config::LightType::POINT)
            ->getColor();
    kwp::Scalar cosTheta = kwp::dot(isect.normal, w);
    kwp::Scalar cosPhi = std::pow(kwp::dot(v, r), isect.material.alpha.value());
    core::math::ColorF diffuseTerm = isect.material.kd;
    core::math::ColorF specularTerm = isect.material.ks.value();
    core::math::ColorF ambientLightColor =
        scene.getSpecificLightByType(core::config::LightType::AMBIENT)
            ->getColor();
    core::math::ColorF ambientCoefficient = isect.material.ka.value();
    return core::math::Color::fromFloat(
        pointLightColor * cosTheta * (diffuseTerm + (specularTerm * cosPhi)) +
        (ambientLightColor * ambientCoefficient));
  }
  return backgroundColor_;
}

} // namespace temprenderer::renderer
