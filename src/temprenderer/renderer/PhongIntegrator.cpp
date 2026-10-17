#include "renderer/PhongIntegrator.hpp"
#include "temprenderer/core/math/Materials.hpp"

#include <algorithm>

namespace temprenderer::renderer {

core::math::Color PhongIntegrator::li(const core::math::Ray &ray,
                                      const scene::Scene &scene) const {
  scene::SurfaceInteraction isect{};
  if (scene.intersect(ray, &isect)) {
    const core::math::ColorF baseColor =
        isect.material.sampleColor(isect.textureContext);
    kwp::Vec3 normal =
        isect.material.sampleNormal(isect.textureContext, isect.normal);
    if (kwp::dot(normal, isect.normal) < 0.0F) {
      normal = -normal;
    }
    core::math::ColorF ambientLightColor{0.0F, 0.0F, 0.0F};
    for (const auto &light : scene.getLights()) {
      if (light->getLightType() == core::config::LightType::AMBIENT) {
        ambientLightColor += light->getColor() * light->getIntensity();
      }
    }
    const core::math::ColorF ambientCoefficient =
        isect.material.ka.value_or(core::math::ColorF{0.0F, 0.0F, 0.0F});
    const bool metallic = isect.material.metallic > 0.0F;
    const core::math::ColorF ambientTerm =
        metallic ? baseColor * ambientLightColor * 1.0F
                 : ambientLightColor * ambientCoefficient;
    kwp::Vec3 v = (ray.getOrigin() - isect.point).normalize();
    const kwp::Scalar alpha = isect.material.alpha.value_or(32.0F);
    const core::math::ColorF specularTerm =
        isect.material.ks.value_or(core::math::ColorF{0.0F, 0.0F, 0.0F});
    const float roughness =
        isect.material.sampleRoughness(isect.textureContext);
    core::math::ColorF result = ambientTerm;
    constexpr unsigned int areaSamplesPerAxis = 4;
    for (const auto &light : scene.getLights()) {
      if (light->getLightType() != core::config::LightType::POINT &&
          light->getLightType() != core::config::LightType::AREA) {
        continue;
      }
      const unsigned int sampleCount =
          light->isAreaLight() ? areaSamplesPerAxis * areaSamplesPerAxis : 1U;
      core::math::ColorF accumulated{0.0F, 0.0F, 0.0F};
      for (unsigned int sample = 0; sample < sampleCount; ++sample) {
        const float u =
            light->isAreaLight()
                ? (static_cast<float>(sample % areaSamplesPerAxis) + 0.5F) /
                      areaSamplesPerAxis
                : 0.5F;
        const float vSample =
            light->isAreaLight()
                ? (static_cast<float>(sample / areaSamplesPerAxis) + 0.5F) /
                      areaSamplesPerAxis
                : 0.5F;
        const kwp::Point3 lightPosition = light->samplePosition(u, vSample);
        const kwp::Vec3 toLight = lightPosition - isect.point;
        const kwp::Scalar distanceToLight = toLight.length();
        if (distanceToLight <= kwp::epsilon) {
          continue;
        }
        const kwp::Vec3 direction = toLight / distanceToLight;
        const kwp::Scalar nDotL = std::max(kwp::dot(normal, direction), 0.0F);
        if (nDotL <= 0.0F) {
          continue;
        }
        if (this->shadows_) {
          const kwp::Point3 shadowOrigin = isect.point + isect.normal * 5e-3F;
          const kwp::Vec3 shadowDirection =
              (lightPosition - shadowOrigin).normalize();
          scene::SurfaceInteraction shadowIsect{};
          if (scene.intersect(core::math::Ray{shadowOrigin, shadowDirection},
                              &shadowIsect) &&
              (shadowIsect.point - shadowOrigin).length() < distanceToLight) {
            continue;
          }
        }
        const core::math::ColorF incident =
            light->getColor() *
            (light->getIntensity() / static_cast<float>(sampleCount));
        if (metallic) {
          const kwp::Scalar nDotV = std::max(kwp::dot(normal, v), 0.0F);
          if (nDotV <= 0.0F) {
            continue;
          }
          kwp::Vec3 halfVector = direction + v;
          if (halfVector.length() <= kwp::epsilon) {
            continue;
          }
          halfVector = halfVector.normalize();
          const auto specular = core::math::evaluateMetalSpecular(
              baseColor, nDotL, nDotV,
              std::max(kwp::dot(normal, halfVector), 0.0F),
              std::max(kwp::dot(v, halfVector), 0.0F), roughness);
          accumulated += incident * specular * nDotL;
        } else {
          const kwp::Vec3 reflection =
              2.0F * kwp::dot(direction, normal) * normal - direction;
          const float highlight =
              std::pow(std::max(kwp::dot(v, reflection), 0.0F), alpha);
          accumulated +=
              incident * nDotL * (baseColor + specularTerm * highlight);
        }
      }
      result += accumulated;
    }
    return core::math::Color::fromFloat(result);
  }
  return backgroundColor_;
}

} // namespace temprenderer::renderer
