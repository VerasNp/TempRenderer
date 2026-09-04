#include "scene/SceneComposer.hpp"
#include "core/logging/LoggerManager.hpp"
#include "renderer/Light.hpp"
#include "renderer/Sphere.hpp"

namespace temprenderer::scene {

namespace {
std::shared_ptr<Hittable>
buildObject(const core::config::ObjectConfig &objectConfig) {
  if (objectConfig.type == core::config::ObjectType::SPHERE) {
    const auto [position, radius] =
        std::get<core::config::SphereConfig>(objectConfig.props);
    return std::make_shared<Sphere>(position, radius, objectConfig.material);
  }
  LC_LOG(core::logging::LogLevel::WARNING, "Object type not recognized");
  return nullptr;
}
} // namespace

Scene SceneComposer::compose(const core::config::SceneConfig &sceneConfig) {
  LC_LOG_VERBOSE(core::logging::LogLevel::INFO, "Scene being composed");
  Scene scene;
  for (const auto &light : sceneConfig.lights) {
    scene.addLight(std::make_shared<renderer::Light>(
        light.type, light.position, light.color, light.intensity));
  }
  for (const auto &object : sceneConfig.objects) {
    if (const auto obj = buildObject(object)) {
      scene.addObject(obj);
    }
  }
  LC_LOG_VERBOSE(core::logging::LogLevel::INFO, "Scene composed successfully");
  return scene;
}
} // namespace temprenderer::scene
