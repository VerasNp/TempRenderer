#include "core/config/SceneConfig.hpp"
#include "core/config/utils.hpp"
#include "core/logging/LoggerManager.hpp"

namespace temprenderer::core::config {

namespace {

/**
 * @brief TODO
 *
 * @param lightType
 * @return
 */
LightType stringToLightType(const std::string &lightType) noexcept {
  if (lightType == "point") {
    return LightType::POINT;
  }
  return LightType::AMBIENT;
}

/**
 * @brief TODO
 *
 * @param light
 * @return
 */
std::vector<LightConfig> loadLightsConfig(const toml::array &nodes) {
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Loading light config");
  std::vector<LightConfig> lights;
  lights.resize(nodes.size());
  for (int i = 0; i < lights.size(); ++i) {
    auto *const light = nodes[i].as_table();
    lights[i].type = stringToLightType((*light)["type"].value_or("point"));
    if (auto *const positionTable = (*light)["position"].as_table()) {
      lights[i].position = parsePoint3DDataFromConfig(*positionTable);
    }
    if (auto *const colorTable = (*light)["color"].as_table()) {
      lights[i].color = parseColorDataFromConfig(*colorTable);
    }
    if (auto *const intensity = (*light)["intensity"].as_floating_point()) {
      lights[i].intensity = intensity->get();
    }
  }
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Light config loaded successfully");
  return lights;
}

/**
 * @brief TODO
 *
 * @param objectType
 * @return
 */
ObjectType stringToObjectType(const std::string &objectType) noexcept {
  if (objectType == "sphere") {
    return ObjectType::SPHERE;
  }
  return ObjectType::OBJECT;
}

/**
 * @brief TODO
 *
 * @param object
 * @param props
 */
void setObjectProps(ObjectConfig &object, const toml::table &props) noexcept {
  if (object.type == ObjectType::SPHERE) {
    if (auto *const position = props["center"].as_table()) {
      get<SphereConfig>(object.props).center =
          parsePoint3DDataFromConfig(*position);
    }
    get<SphereConfig>(object.props).radius =
        props["radius"].value_or(get<SphereConfig>(object.props).radius);
  }
}

/**
 * @brief TODO
 *
 * @param material
 * @param props
 */
void setMaterialProps(math::Material &material,
                      const toml::table &props) noexcept {
  if (auto *const diffuse = props["kd"].as_table()) {
    material.kd = parseColorDataFromConfig(*diffuse);
  }
  if (auto *const specular = props["ks"].as_table()) {
    material.ks = parseColorDataFromConfig(*specular);
  }
  if (auto *const ambient = props["ka"].as_table()) {
    material.ka = parseColorDataFromConfig(*ambient);
  }
  material.alpha = props["alpha"].value_or(0);
}

/**
 * @brief TODO
 *
 * @param nodes
 * @return
 */
std::vector<ObjectConfig> loadObjectsConfig(const toml::array &nodes) {
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Loading objects config");
  std::vector<ObjectConfig> objects;
  objects.resize(nodes.size());
  for (int i = 0; i < objects.size(); ++i) {
    auto *const object = nodes[i].as_table();
    auto objectType = (*object)["type"].value<std::string>();
    objects[i].type = stringToObjectType(objectType.value());
    auto *objectProps = (*object)["props"].as_table();
    setObjectProps(objects[i], *objectProps);
    auto *material = (*object)["material"].as_table();
    auto materialType = (*material)["type"].value<std::string>();
    auto *materialProps = (*material)["props"].as_table();
    setMaterialProps(objects[i].material, *materialProps);
  }
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Objects config loaded successfully");
  return objects;
}
} // namespace

[[nodiscard]] std::string
SceneConfig::objectTypeToString(const ObjectType objectType) noexcept {
  if (objectType == ObjectType::SPHERE) {
    return "sphere";
  }
  return "object";
}

SceneConfig SceneConfig::loadSceneConfig(const toml::table &table) {
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Loading scene config");
  SceneConfig config;
  if (auto *const light = table["lights"].as_array()) {
    config.lights = loadLightsConfig(*light);
  }
  if (auto *const objects = table["objects"].as_array()) {
    config.objects = loadObjectsConfig(*objects);
  }
  return config;
};
} // namespace temprenderer::core::config
