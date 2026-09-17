#pragma once
#include "core/math/Materials.hpp"
#include "core/parsers/ParserPort.hpp"
#include "kwp/Point3.hpp"
#include <variant>
#include <vector>

namespace temprenderer::core::config {

/**
 * Possible types of light
 */
enum class LightType { POINT, AMBIENT };

/**
 * @brief Light configs
 */
struct LightConfig {
  LightType type = LightType::POINT;
  kwp::Point3 position = kwp::Point3(0, 0, 0);
  math::ColorF color = math::ColorF(1.0F, 1.0F, 1.0F);
  float intensity = 10.0F;
};

/**
 * Possible types of objects
 */
enum class ObjectType { SPHERE, OBJECT, CONE, PLANE, CYLINDER };

/**
 * @brief Sphere configs
 */
struct SphereConfig {
  kwp::Point3 center{0, 0, 0};
  kwp::Scalar radius = 1.0F;
};

struct ConeConfig {
  kwp::Point3 baseCenter{0, 0, 0};
  float baseRadius = 1.0F;
  kwp::Point3 vertex{0, 0, 0};
};

struct PlaneConfig {
  kwp::Point3 point{0, 0, 0};
  kwp::Vec3 normal{0, 1, 0};
};

struct CylinderConfig {
  kwp::Point3 baseCenter{0, 0, 0};
  float baseRadius = 1.0F;
  kwp::Point3 topCenter{0, 0, 0};
};

using ObjectProps =
    std::variant<SphereConfig, ConeConfig, PlaneConfig, CylinderConfig>;

/**
 * @brief Object configs
 */
struct ObjectConfig {
  ObjectType type = ObjectType::SPHERE;
  ObjectProps props;
  math::Material material;
};

/**
 * @brief Scene configs
 */
struct SceneConfig {
  std::vector<LightConfig> lights;
  std::vector<ObjectConfig> objects;

  /**
   * @brief TODO
   *
   * @param sceneConfig
   * @return
   */
  static SceneConfig loadSceneConfig(const parsers::ConfigValue &sceneConfig);
};
} // namespace temprenderer::core::config
