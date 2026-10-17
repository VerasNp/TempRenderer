#include "core/config/SceneConfig.hpp"
#include "core/logging/LoggerManager.hpp"

namespace temprenderer::core::config {

SceneConfig
SceneConfig::loadConfig(const parsers::ConfigValue &config,
                        const std::filesystem::path &baseDirectory) {
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Loading scene config");
  SceneConfig scene;
  if (const auto *worldConfig = config.get("world")) {
    scene.world = WorldConfig::loadConfig(*worldConfig);
  }
  if (const auto *cameraConfig = config.get("camera")) {
    scene.camera = CameraConfig::loadConfig(*cameraConfig);
  }
  if (const auto lightsValue = config.get("lights")) {
    for (const auto &lightConfig : lightsValue->asArray()) {
      scene.lights.push_back(LightConfig::loadConfig(lightConfig));
    }
  }
  if (const auto objectsValue = config.get("objects")) {
    for (const auto &objectConfig : objectsValue->asArray()) {
      scene.objects.push_back(
          ObjectConfig::loadConfig(objectConfig, baseDirectory));
    }
  }
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Scene config loaded successfully");
  return scene;
}
} // namespace temprenderer::core::config
