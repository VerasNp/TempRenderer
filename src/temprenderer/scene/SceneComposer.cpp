#include "scene/SceneComposer.hpp"
#include "core/logging/LoggerManager.hpp"
#include "renderer/Light.hpp"
#include "renderer/Sphere.hpp"

namespace temprenderer::scene {

namespace {
std::shared_ptr<core::math::Material>
buildMaterial(const core::config::MaterialConfig &materialConfig) {
  switch (materialConfig.type) {
  case core::config::MaterialType::DIFFUSE_ONLY: {
    const auto &cfg =
        std::get<core::config::DiffuseOnlyMaterialConfig>(materialConfig.props);
    return std::make_shared<core::math::DiffuseOnlyMaterial>(cfg.kd);
  }
  case core::config::MaterialType::DIFFUSE_SPECULAR: {
    const auto &cfg = std::get<core::config::DiffuseSpecularMaterialConfig>(
        materialConfig.props);
    return std::make_shared<core::math::DiffuseSpecularMaterial>(cfg.kd, cfg.ks,
                                                                 cfg.alpha);
  }
  }
  LC_LOG(core::logging::LogLevel::WARNING,
         "Material type not recognized, using default");
  return nullptr;
}

std::shared_ptr<Hittable>
buildObject(const core::config::ObjectConfig &objectConfig) {
  if (objectConfig.type == core::config::ObjectType::SPHERE) {
    const auto [position, radius] =
        std::get<core::config::SphereConfig>(objectConfig.props);
    return std::make_shared<Sphere>(position, radius,
                                    buildMaterial(objectConfig.material));
  }
  LC_LOG(core::logging::LogLevel::WARNING, "Object type not recognized");
  return nullptr;
}
} // namespace

Scene SceneComposer::compose(const core::config::SceneConfig &sceneConfig) {
  LC_LOG_VERBOSE(core::logging::LogLevel::INFO, "Scene being composed");
  Scene scene;
  renderer::Light light(sceneConfig.light.position, sceneConfig.light.color,
                        sceneConfig.light.intensity);
  scene.setLight(std::make_shared<renderer::Light>(light));
  for (const auto &object : sceneConfig.objects) {
    if (const auto obj = buildObject(object)) {
      scene.add(obj);
    }
  }
  LC_LOG_VERBOSE(core::logging::LogLevel::INFO, "Scene composed successfully");
  return scene;
}
} // namespace temprenderer::scene
