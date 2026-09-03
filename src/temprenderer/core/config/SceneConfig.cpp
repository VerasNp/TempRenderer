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
 * @param materialType
 * @return
 */
MaterialType stringToMaterialType(const std::string &materialType) noexcept {
  if (materialType == "diffuse_only") {
    return MaterialType::DIFFUSE_ONLY;
  }
  return MaterialType::DIFFUSE_SPECULAR;
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
void setMaterialProps(MaterialConfig &material,
                      const toml::table &props) noexcept {
  if (material.type == MaterialType::DIFFUSE_ONLY) {
    if (auto *const diffuse = props["kd"].as_table()) {
      get<DiffuseOnlyMaterialConfig>(material.props).kd =
          parseColorDataFromConfig(*diffuse);
    }
  } else if (material.type == MaterialType::DIFFUSE_SPECULAR) {
    if (auto *const diffuse = props["kd"].as_table()) {
      get<DiffuseSpecularMaterialConfig>(material.props).kd =
          parseColorDataFromConfig(*diffuse);
    }
    if (auto *const diffuse = props["ks"].as_table()) {
      get<DiffuseSpecularMaterialConfig>(material.props).kd =
          parseColorDataFromConfig(*diffuse);
    }
    get<DiffuseSpecularMaterialConfig>(material.props).alpha =
        props["alpha"].value_or(
            get<DiffuseSpecularMaterialConfig>(material.props).alpha);
  }
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
    objects[i].material.type = stringToMaterialType(*materialType);
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
std::string
SceneConfig::materialTypeToString(const MaterialType materialType) noexcept {
  if (materialType == MaterialType::DIFFUSE_ONLY) {
    return "diffuse_only";
  }
  return "diffuse_specular";
}

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
