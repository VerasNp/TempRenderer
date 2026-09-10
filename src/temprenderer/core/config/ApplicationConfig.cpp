#include "temprenderer/core/config/ApplicationConfig.hpp"
#include "core/config/RenderConfig.hpp"
#include "core/config/WindowConfig.hpp"
#include "core/config/utils.hpp"
#include "core/parsers/ParserPort.hpp"
#include "temprenderer/core/logging/LoggerManager.hpp"
#include <format>
#include <string>

namespace temprenderer::core::config {

namespace {
std::string formatObjectConfig(const ObjectConfig &obj, size_t index) {
  std::string propsStr = std::visit(
      []<typename T0>(const T0 &props) -> std::string {
        using T = std::decay_t<T0>;
        if constexpr (std::is_same_v<T, SphereConfig>) {
          return std::format("pos=({:.1f}, {:.1f}, {:.1f}) radius={:.1f}",
                             props.center.x, props.center.y, props.center.z,
                             props.radius);
        } else {
          return "props=<unknown>";
        }
      },
      obj.props);
  return std::format("  [{}] type={}\n"
                     "    props=({})\n"
                     "    material=(ka=({}, {}, {}), kd=({}, {}, {}), ks=({}, "
                     "{}, {}), alpha={})\n",
                     index, objectTypeToString(obj.type), propsStr,
                     obj.material.ka->r, obj.material.ka->g, obj.material.ka->b,
                     obj.material.kd.r, obj.material.kd.g, obj.material.kd.b,
                     obj.material.ks->r, obj.material.ks->g, obj.material.ks->b,
                     obj.material.alpha.value_or(0));
}

std::string formatLightConfig(const LightConfig &light, size_t index) {
  return std::format("  [{}] type={}\n"
                     "    position=({:.1f}, {:.1f}, {:.1f})\n"
                     "    intensity={:.1f}\n"
                     "    color=({}, {}, {})\n",
                     index, lightTypeToString(light.type), light.position.x,
                     light.position.y, light.position.z, light.intensity,
                     light.color.r, light.color.g, light.color.b);
}

void logConfigVerbose(const ApplicationConfig &config) {
  std::string lightsStr;
  for (size_t i = 0; i < config.scene.lights.size(); ++i) {
    lightsStr += formatLightConfig(config.scene.lights[i], i);
  }

  std::string objectsStr;
  for (size_t i = 0; i < config.scene.objects.size(); ++i) {
    objectsStr += formatObjectConfig(config.scene.objects[i], i);
  }

  LC_LOG_VERBOSE(
      logging::LogLevel::INFO,
      std::format("\n===== ApplicationConfig (verbose) =====\n"
                  "[window]\n"
                  "  title  = {}\n"
                  "  width  = {}\n"
                  "  height = {}\n"
                  "[render]\n"
                  "  resolution_width  = {}\n"
                  "  resolution_height = {}\n"
                  "  aspect_ratio      = {}\n"
                  "  viewport_width    = {}\n"
                  "  viewport_height   = {}\n"
                  "[camera]\n"
                  "  eye.x        = {}\n"
                  "  eye.y        = {}\n"
                  "  eye.z        = {}\n"
                  "  focal_length = {}\n"
                  "[scene.light] ({} total)\n"
                  "{}"
                  "[scene.objects] ({} total)\n"
                  "{}"
                  "========================================",
                  config.window.title, config.window.width,
                  config.window.height, config.render.resolutionWidth,
                  config.render.resolutionHeight,
                  aspectRatioToScalar(config.render.aspectRatio),
                  config.render.viewportWidth, config.render.viewportHeight,
                  config.camera.eye.x, config.camera.eye.y, config.camera.eye.z,
                  config.camera.focalLength, config.scene.lights.size(),
                  lightsStr, config.scene.objects.size(), objectsStr));
}
} // namespace

ApplicationConfig ApplicationConfig::loadFromFile(const std::string &path) {

  const std::unique_ptr<parsers::ParserPort> parser =
      std::make_unique<parsers::TOMLPlusPlusParserAdapter>();
  const parsers::ConfigValue root = parser->parserFile(path);
  ApplicationConfig config;
  try {
    if (const auto windowConfig = root.get("window")) {
      config.window = WindowConfig::loadWindowConfig(*windowConfig);
    }
    if (const auto renderConfig = root.get("render")) {
      config.render = RenderConfig::loadRenderConfig(*renderConfig);
    }
    if (const auto camera = root.get("camera")) {
      config.camera = CameraConfig::loadCameraConfig(*camera);
    }
    if (const auto scene = root.get("scene")) {
      config.scene = SceneConfig::loadSceneConfig(*scene);
    }
  } catch (const std::exception &err) {
    LC_LOG(logging::LogLevel::ERROR,
           std::string("Unexpected error while loading config '") + path +
               "': " + err.what());
    throw;
  }
  logConfigVerbose(config);
  LC_LOG_VERBOSE(logging::LogLevel::INFO,
                 "Application config loaded successfully");
  return config;
}
} // namespace temprenderer::core::config
