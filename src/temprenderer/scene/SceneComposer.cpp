#include "scene/SceneComposer.hpp"

#include "core/logging/LoggerManager.hpp"
#include "renderer/Sphere.hpp"
namespace temprenderer::scene {

namespace {
std::shared_ptr<Hittable>
buildObject(const core::config::ObjectConfig &objectConfig) {
  if (objectConfig.type == core::config::ObjectType::SPHERE) {
    const auto [position, radius] =
        std::get<core::config::SphereConfig>(objectConfig.props);
    return std::make_shared<Sphere>(position, radius, objectConfig.color);
  }
  LC_LOG(core::logging::LogLevel::WARNING, "Object type not recognized");
  return nullptr;
}
} // namespace

Scene SceneComposer::compose(const core::config::SceneConfig &sceneConfig) {
  Scene scene;
  for (const auto &object : sceneConfig.objects) {
    if (const auto obj = buildObject(object)) {
      scene.add(obj);
    }
  }
  return scene;
}
} // namespace temprenderer::scene
