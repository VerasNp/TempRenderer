#include "core/config/SceneConfig.hpp"
#include "core/logging/LoggerManager.hpp"

namespace temprenderer::core::config {

SceneConfig SceneConfig::loadConfig(const parsers::ConfigValue &config) {
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Loading scene config");
  SceneConfig scene;
  if (const auto *worldConfig = config.get("world")) {
    scene.world = WorldConfig::loadConfig(*worldConfig);
  }
  if (const auto *cameraConfig = config.get("camera")) {
    scene.camera = CameraConfig::loadConfig(*cameraConfig);
  }
  if (const auto lightsConfig = config.get("lights")->asArray();
      !lightsConfig.empty()) {
    for (auto lightConfig : lightsConfig) {
      scene.lights.push_back(LightConfig::loadConfig(lightConfig));
    }
  }
  if (const auto objectsConfig = config.get("objects")->asArray();
      !objectsConfig.empty()) {
    for (auto objectConfig : objectsConfig) {
      scene.objects.push_back(ObjectConfig::loadConfig(objectConfig));
    }
  }
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Scene config loaded successfully");
  return scene;
}
} // namespace temprenderer::core::config
