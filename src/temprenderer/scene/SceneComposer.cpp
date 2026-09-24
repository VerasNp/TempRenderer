#include "scene/SceneComposer.hpp"
#include "core/logging/LoggerManager.hpp"
#include "renderer/Cone.hpp"
#include "renderer/Cylinder.hpp"
#include "renderer/Light.hpp"
#include "renderer/Plane.hpp"
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
  if (objectConfig.type == core::config::ObjectType::CONE) {
    const auto [baseCenter, baseRadius, vertex] =
        std::get<core::config::ConeConfig>(objectConfig.props);
    return std::make_shared<renderer::Cone>(baseCenter, baseRadius, vertex,
                                            objectConfig.material);
  }
  if (objectConfig.type == core::config::ObjectType::PLANE) {
    const auto [point, normal] =
        std::get<core::config::PlaneConfig>(objectConfig.props);
    return std::make_shared<renderer::Plane>(point, normal,
                                             objectConfig.material);
  }
  if (objectConfig.type == core::config::ObjectType::CYLINDER) {
    const auto [baseCenter, baseRadius, height] =
        std::get<core::config::CylinderConfig>(objectConfig.props);
    return std::make_shared<renderer::Cylinder>(baseCenter, baseRadius, height,
                                                objectConfig.material);
  }
  LC_LOG(core::logging::LogLevel::WARNING, "Object type not recognized");
  return nullptr;
}
} // namespace

Scene SceneComposer::compose(const core::config::SceneConfig &sceneConfig,
                             const core::config::RenderConfig &renderConfig) {
  LC_LOG_VERBOSE(core::logging::LogLevel::INFO, "Scene being composed");
  Scene scene;
  for (const auto &component : sceneConfig.collection) {
    if (component.type == core::config::SceneComponentType::LIGHT) {
      const auto [type, position, color, intensity] =
          std::get<core::config::LightConfig>(component.props);
      scene.addLight(
          std::make_shared<renderer::Light>(type, position, color, intensity));
    } else if (component.type == core::config::SceneComponentType::OBJECT) {
      const auto objectConfig =
          std::get<core::config::ObjectConfig>(component.props);
      const auto object = buildObject(objectConfig);
      scene.addObject(object);
    } else if (component.type == core::config::SceneComponentType::CAMERA) {
      const auto [eye, focalLength] =
          std::get<core::config::CameraConfig>(component.props);
      scene.addCamera(std::make_shared<renderer::Camera>(
          eye, focalLength, renderConfig.resolutionHeight,
          renderConfig.resolutionWidth, renderConfig.viewportWidth,
          renderConfig.viewportHeight));
    }
  }
  LC_LOG_VERBOSE(core::logging::LogLevel::INFO, "Scene composed successfully");
  return scene;
}
} // namespace temprenderer::scene
