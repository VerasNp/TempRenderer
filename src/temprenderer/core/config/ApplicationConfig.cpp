#include "temprenderer/core/config/ApplicationConfig.hpp"

#include "core/config/RenderConfig.hpp"
#include "core/config/WindowConfig.hpp"
#include "temprenderer/core/logging/LoggerManager.hpp"

#include <format>
#include <toml++/toml.hpp>

#include <string>

namespace temprenderer::core::config {

namespace {
// std::string formatObjectConfig(const ObjectConfig &obj, size_t index) {
//   std::string propsStr = std::visit(
//       []<typename T0>(const T0 &props) -> std::string {
//         using T = std::decay_t<T0>;
//         if constexpr (std::is_same_v<T, SphereConfig>) {
//           return std::format("pos=({:.1f}, {:.1f}, {:.1f}) radius={:.1f}",
//                              props.center.x, props.center.y, props.center.z,
//                              props.radius);
//         } else {
//           return "props=<unknown>";
//         }
//       },
//       obj.props);
//   return std::format(
//       "  [{}] type={}\n"
//       "    props=({})\n"
//       "    material=(kd=({}, {}, {}), ks=({}, {}, {}), alpha={})\n",
//       index, SceneConfig::objectTypeToString(obj.type), propsStr,
//       obj.material.kd.r, obj.material.kd.g, obj.material.kd.b,
//       obj.material.ks.r, obj.material.ks.g, obj.material.ks.b,
//       obj.material.alpha);
// }

// void logConfigVerbose(const ApplicationConfig &config) {
//   std::string objectsStr;
//   for (size_t i = 0; i < config.scene.objects.size(); ++i) {
//     objectsStr += formatObjectConfig(config.scene.objects[i], i);
//   }
//
//   LC_LOG_VERBOSE(
//       logging::LogLevel::INFO,
//       std::format("\n===== ApplicationConfig (verbose) =====\n"
//                   "[window]\n"
//                   "  title  = {}\n"
//                   "  width  = {}\n"
//                   "  height = {}\n"
//                   "[render]\n"
//                   "  resolution_width  = {}\n"
//                   "  resolution_height = {}\n"
//                   "  aspect_ratio      = {}\n"
//                   "  viewport_width    = {}\n"
//                   "  viewport_height   = {}\n"
//                   "[camera]\n"
//                   "  eye.x        = {}\n"
//                   "  eye.y        = {}\n"
//                   "  eye.z        = {}\n"
//                   "  focal_length = {}\n"
//                   "[scene.light]\n"
//                   "  position = ({}, {}, {})\n"
//                   "  color = ({}, {}, {})\n"
//                   "  intensity = {}\n"
//                   "[scene.objects] ({} total)\n"
//                   "{}"
//                   "========================================",
//                   config.window.title, config.window.width,
//                   config.window.height, config.render.resolutionWidth,
//                   config.render.resolutionHeight,
//                   RenderConfig::aspectRatioToScalar(config.render.aspectRatio),
//                   config.render.viewportWidth, config.render.viewportHeight,
//                   config.camera.eye.x, config.camera.eye.y, config.camera.eye.z,
//                   config.camera.focalLength, config.scene.light.position.x,
//                   config.scene.light.position.y, config.scene.light.position.z,
//                   config.scene.light.color.r, config.scene.light.color.g,
//                   config.scene.light.color.b, config.scene.light.intensity,
//                   config.scene.objects.size(), objectsStr));
// }
} // namespace

ApplicationConfig ApplicationConfig::loadFromFile(const std::string &path) {
  toml::table table;
  try {
    LC_LOG_VERBOSE(logging::LogLevel::INFO, "Loading config from " + path);
    table = toml::parse_file(path);
  } catch (const toml::parse_error &err) {
    LC_LOG(logging::LogLevel::ERROR, "Failed to parse config file: " + path);
    throw std::runtime_error(std::string("Erro no arquivo de config: ") +
                             err.description().data());
  }
  ApplicationConfig config;
  try {
    if (auto *const window = table["window"].as_table()) {
      config.window = WindowConfig::loadWindowConfig(*window);
    }
    if (auto *const render = table["render"].as_table()) {
      config.render = RenderConfig::loadRenderConfig(*render);
    }
    if (auto *const camera = table["camera"].as_table()) {
      config.camera = CameraConfig::loadCameraConfig(*camera);
    }
    if (auto *const scene = table["scene"].as_table()) {
      config.scene = SceneConfig::loadSceneConfig(*scene);
    }
  } catch (const std::exception &err) {
    LC_LOG(logging::LogLevel::ERROR,
           std::string("Unexpected error while loading config '") + path +
               "': " + err.what());
    throw;
  }
  // logConfigVerbose(config);
  LC_LOG_VERBOSE(logging::LogLevel::INFO,
                 "Application config loaded successfully");
  return config;
}
} // namespace temprenderer::core::config
