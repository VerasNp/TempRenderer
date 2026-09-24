#include "core/config/ObjectConfig.hpp"

#include "core/config/utils.hpp"
#include "core/parsers/ParserPort.hpp"
namespace temprenderer::core::config {
[[nodiscard]] ObjectConfig
ObjectConfig::loadConfig(const parsers::ConfigValue &config) noexcept {
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Loading objects config");
  ObjectConfig object;
  object.type = stringToObjectType(config.get("type")->asString());
  object.props = createDefaultObjectProps(object.type);
  setObjectProps(object, config);
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Objects config loaded successfully");
  return object;
}

[[nodiscard]] ObjectType
ObjectConfig::stringToObjectType(const std::string &objectType) noexcept {
  if (objectType == "sphere") {
    return ObjectType::SPHERE;
  }
  if (objectType == "cone") {
    return ObjectType::CONE;
  }
  if (objectType == "plane") {
    return ObjectType::PLANE;
  }
  if (objectType == "cylinder") {
    return ObjectType::CYLINDER;
  }
  LC_LOG(logging::LogLevel::WARNING,
         "Unknown object type '" + objectType + "'");
  return ObjectType::UNKNOWN;
}

[[nodiscard]] ObjectProps
ObjectConfig::createDefaultObjectProps(const ObjectType type) noexcept {
  switch (type) {
  case ObjectType::SPHERE:
    return SphereConfig{};
  case ObjectType::CONE:
    return ConeConfig{};
  case ObjectType::PLANE:
    return PlaneConfig{};
  case ObjectType::CYLINDER:
    return CylinderConfig{};
  default:;
    return std::monostate{};
  }
}

/**
 * @brief TODO
 *
 * @param object
 * @param propsConfig
 */
void ObjectConfig::setObjectProps(
    ObjectConfig &object, const parsers::ConfigValue &propsConfig) noexcept {
  if (object.type == ObjectType::UNKNOWN) {
    exit(1);
  }
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
  if (const auto materialConfig = propsConfig.get("material")) {
    if (const auto diffuseConfig = materialConfig->get("kd")) {
      object.material.kd = parseColorDataFromConfig(*diffuseConfig);
    }
    if (const auto specularConfig = materialConfig->get("ks")) {
      object.material.ks = parseColorDataFromConfig(*specularConfig);
    }
    if (const auto ambientConfig = materialConfig->get("ka")) {
      object.material.ka = parseColorDataFromConfig(*ambientConfig);
    }
    object.material.alpha = materialConfig->get("alpha")->asFloat();
  }
}
} // namespace temprenderer::core::config
