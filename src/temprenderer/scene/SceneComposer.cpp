#include "scene/SceneComposer.hpp"

#include "core/io/ObjLoader.hpp"
#include "core/logging/LoggerManager.hpp"
#include "renderer/Cone.hpp"
#include "renderer/Cylinder.hpp"
#include "renderer/Light.hpp"
#include "renderer/Plane.hpp"
#include "renderer/Sphere.hpp"
#include "renderer/Triangle.hpp"
#include "scene/Mesh.hpp"

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
  if (objectConfig.type == core::config::ObjectType::TRIANGLE) {
    const auto [p1, p2, p3] =
        std::get<core::config::TriangleConfig>(objectConfig.props);
    return std::make_shared<renderer::Triangle>(p1, p2, p3,
                                                objectConfig.material);
  }
  if (objectConfig.type == core::config::ObjectType::MESH) {
    const auto &meshConfig =
        std::get<core::config::MeshConfig>(objectConfig.props);
    if (!meshConfig.objFilePath.empty()) {
      core::config::MeshConfig meshConfigFromFile =
          io::loadObjMeshConfig(meshConfig.objFilePath);
      std::vector<std::size_t> indices;
      for (const auto &face : meshConfigFromFile.faces) {
        for (std::size_t i = 1; i + 1 < face.size(); ++i) {
          indices.push_back(static_cast<std::size_t>(face[0]));
          indices.push_back(static_cast<std::size_t>(face[i]));
          indices.push_back(static_cast<std::size_t>(face[i + 1]));
        }
      }
      return std::make_shared<Mesh>(meshConfigFromFile.vertices,
                                    std::move(indices), objectConfig.material);
    }
    std::vector<std::size_t> indices;
    for (const auto &face : meshConfig.faces) {
      for (std::size_t i = 1; i + 1 < face.size(); ++i) {
        indices.push_back(static_cast<std::size_t>(face[0]));
        indices.push_back(static_cast<std::size_t>(face[i]));
        indices.push_back(static_cast<std::size_t>(face[i + 1]));
      }
    }
    return std::make_shared<Mesh>(meshConfig.vertices, std::move(indices),
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
  scene.setBackgroundColor(sceneConfig.world.background.color);
  scene.addCamera(std::make_shared<renderer::Camera>(
      sceneConfig.camera.name, sceneConfig.camera.eye,
      sceneConfig.camera.focalLength, renderConfig.resolutionWidth,
      renderConfig.resolutionHeight, renderConfig.viewportWidth,
      renderConfig.viewportHeight));
  for (auto [name, type, position, color, intensity] : sceneConfig.lights) {
    scene.addLight(std::make_shared<renderer::Light>(name, type, position,
                                                     color, intensity));
  }
  for (auto objectConfig : sceneConfig.objects) {
    scene.addObject(buildObject(objectConfig));
  }
  LC_LOG_VERBOSE(core::logging::LogLevel::INFO, "Scene composed successfully");
  return scene;
}
} // namespace temprenderer::scene
