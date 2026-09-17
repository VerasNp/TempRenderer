#include "core/config/SceneConfig.hpp"
#include "core/config/utils.hpp"
#include "core/logging/LoggerManager.hpp"

namespace temprenderer::core::config {

namespace {
/**
 * @brief TODO
 *
 * @param lightsConfig
 * @return
 */
std::vector<LightConfig>
loadLightsConfig(const parsers::ConfigValue &lightsConfig) {
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Loading light config");
  std::vector<LightConfig> lights;
  for (const auto &lightConfig : lightsConfig.asArray()) {
    LightConfig light;
    light.type = stringToLightType(lightConfig.get("type")->asString());
    if (const auto positionConfig = lightConfig.get("position")) {
      light.position = parsePoint3DDataFromConfig(*positionConfig);
    }
    if (const auto colorConfig = lightConfig.get("color")) {
      light.color = parseColorDataFromConfig(*colorConfig);
    }
    light.intensity = lightConfig.get("intensity")->asFloat(light.intensity);
    lights.push_back(light);
  }
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Light config loaded successfully");
  return lights;
}

/**
 * @brief TODO
 *
 * @param object
 * @param propsConfig
 */
void setObjectProps(ObjectConfig &object,
                    const parsers::ConfigValue &propsConfig) noexcept {
  if (object.type == ObjectType::SPHERE) {
    auto &sphere = std::get<SphereConfig>(object.props);
    if (const auto position = propsConfig.get("center")) {
      sphere.center = parsePoint3DDataFromConfig(*position);
    }
    if (const auto radiusOpt = propsConfig.get("radius")) {
      sphere.radius = radiusOpt->asFloat(sphere.radius);
    }
  } else if (object.type == ObjectType::CONE) {
    auto &cone = std::get<ConeConfig>(object.props);
    if (const auto position = propsConfig.get("base_center")) {
      cone.baseCenter = parsePoint3DDataFromConfig(*position);
    }
    if (const auto position = propsConfig.get("vertex")) {
      cone.vertex = parsePoint3DDataFromConfig(*position);
    }
    if (const auto radiusOpt = propsConfig.get("base_radius")) {
      cone.baseRadius = radiusOpt->asFloat(cone.baseRadius);
    }
  } else if (object.type == ObjectType::PLANE) {
    auto &plane = std::get<PlaneConfig>(object.props);
    if (const auto position = propsConfig.get("point")) {
      plane.point = parsePoint3DDataFromConfig(*position);
    }
    if (const auto vector = propsConfig.get("normal")) {
      plane.normal = parseVector3DDataFromConfig(*vector);
    }
  } else if (object.type == ObjectType::CYLINDER) {
    auto &cylinder = std::get<CylinderConfig>(object.props);
    if (const auto position = propsConfig.get("base_center")) {
      cylinder.baseCenter = parsePoint3DDataFromConfig(*position);
    }
    if (const auto radiusOpt = propsConfig.get("base_radius")) {
      cylinder.baseRadius = radiusOpt->asFloat(cylinder.baseRadius);
    }
    if (const auto position = propsConfig.get("top_center")) {
      cylinder.topCenter = parsePoint3DDataFromConfig(*position);
    }
  }
}

/**
 * @brief TODO
 *
 * @param material
 * @param materialConfig
 */
void setMaterialProps(math::Material &material,
                      const parsers::ConfigValue &materialConfig) noexcept {
  if (const auto diffuseConfig = materialConfig.get("kd")) {
    material.kd = parseColorDataFromConfig(*diffuseConfig);
  }
  if (const auto specularConfig = materialConfig.get("ks")) {
    material.ks = parseColorDataFromConfig(*specularConfig);
  }
  if (const auto ambientConfig = materialConfig.get("ka")) {
    material.ka = parseColorDataFromConfig(*ambientConfig);
  }
  material.alpha = materialConfig.get("alpha")->asFloat();
}

/**
 * @brief TODO
 *
 * @param objectsConfig
 * @return
 */
std::vector<ObjectConfig>
loadObjectsConfig(const parsers::ConfigValue &objectsConfig) {
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Loading objects config");
  std::vector<ObjectConfig> objects;
  for (const auto &objectConfig : objectsConfig.asArray()) {
    ObjectConfig object;
    object.type = stringToObjectType(objectConfig.get("type")->asString());
    object.props = createDefaultProps(object.type);
    setObjectProps(object, *objectConfig.get("props"));
    setMaterialProps(object.material,
                     *objectConfig.get("material")->get("props"));
    objects.push_back(object);
  }
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Objects config loaded successfully");
  return objects;
}
} // namespace
SceneConfig
SceneConfig::loadSceneConfig(const parsers::ConfigValue &sceneConfig) {
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Loading scene config");
  SceneConfig config;
  if (const auto lightsConfig = sceneConfig.get("lights");
      lightsConfig->isArray()) {
    config.lights = loadLightsConfig(*lightsConfig);
  }
  if (auto *const objects = sceneConfig.get("objects")) {
    config.objects = loadObjectsConfig(*objects);
  }
  return config;
};
} // namespace temprenderer::core::config
