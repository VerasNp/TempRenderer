#include "core/config/SceneConfig.hpp"
#include "core/config/utils.hpp"
#include "core/logging/LoggerManager.hpp"

namespace temprenderer::core::config {

namespace {
/**
 * @brief TODO
 *
 * @param light
 * @return
 */
LightConfig loadLightConfig(const toml::table &light) {
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Loading light config");
  LightConfig config;
  if (auto *const positionTable = light["position"].as_table()) {
    config.position = parsePoint3DDataFromConfig(*positionTable);
  }
  if (auto *const colorTable = light["color"].as_table()) {
    config.color = parseColorDataFromConfig(*colorTable);
  }
  if (auto *const intensity = light["intensity"].as_floating_point()) {
    config.intensity = intensity->get();
  }
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Light config loaded successfully");
  return config;
}

ObjectType stringToObjectType(const std::string &objectType) noexcept {
  if (objectType == "sphere") {
    return ObjectType::SPHERE;
  }
  return ObjectType::OBJECT;
}

void setObjectProps(ObjectConfig &object, const toml::table &props) noexcept {
  if (object.type == ObjectType::SPHERE) {
    if (auto *const position = props["position"].as_table()) {
      get<SphereConfig>(object.props).center =
          parsePoint3DDataFromConfig(*position);
    }
    get<SphereConfig>(object.props).radius =
        props["radius"].value_or(get<SphereConfig>(object.props).radius);
  }
}

void setMaterialConfig(ObjectConfig &object, const toml::table &material) {
  if (auto *const kd = material["kd"].as_table()) {
    object.material.kd = parseColorDataFromConfig(*kd);
  }
  if (auto *const ks = material["ks"].as_table()) {
    object.material.ks = parseColorDataFromConfig(*ks);
  }
  object.material.alpha = material["alpha"].value_or(1);
}

std::vector<ObjectConfig> loadObjectsConfig(const toml::array &nodes) {
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Loading objects config");
  std::vector<ObjectConfig> objects;
  objects.resize(nodes.size());
  for (int i = 0; i < objects.size(); ++i) {
    auto *const object = nodes[i].as_table();
    auto type = (*object)["type"].value<std::string>();
    objects[i].type = stringToObjectType(type.value());
    auto *props = (*object)["props"].as_table();
    setObjectProps(objects[i], *props);
    auto *material = (*object)["material"].as_table();
    setMaterialConfig(objects[i], *material);
  }
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Objects config loaded successfully");
  return objects;
}
} // namespace

/**
 * @brief TODO
 *
 * @param table
 * @return
 */
SceneConfig SceneConfig::loadSceneConfig(const toml::table &table) {
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Loading scene config");
  SceneConfig config;
  if (auto *const light = table["light"].as_table()) {
    config.light = loadLightConfig(*light);
  }
  if (auto *const objects = table["objects"].as_array()) {
    config.objects = loadObjectsConfig(*objects);
  }
  return config;
};
} // namespace temprenderer::core::config
