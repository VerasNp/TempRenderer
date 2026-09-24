#pragma once

#include "core/math/Materials.hpp"
#include "core/parsers/ParserPort.hpp"
#include "kwp/Point3.hpp"
#include <string>
#include <variant>

namespace temprenderer::core::config {
/**
 * Possible types of objects
 */
enum class ObjectType { SPHERE, OBJECT, CONE, PLANE, CYLINDER, UNKNOWN };

/**
 * @brief Sphere configs
 */
struct SphereConfig {
  kwp::Point3 center{0, 0, 0};
  kwp::Scalar radius = 0.0F;
};

/**
 * @brief Cone configs
 */
struct ConeConfig {
  kwp::Point3 baseCenter{0, 0, 0};
  float baseRadius = 0.0F;
  kwp::Point3 vertex{0, 0, 0};
};

/**
 * @brief Plane configs
 */
struct PlaneConfig {
  kwp::Point3 point{0, 0, 0};
  kwp::Vec3 normal{0, 1, 0};
};

/**
 * @brief Cylinder configs
 */
struct CylinderConfig {
  kwp::Point3 baseCenter{0, 0, 0};
  float baseRadius = 1.0F;
  kwp::Point3 topCenter{0, 0, 0};
};

/**
 * @brief Possible types of object's props
 */
using ObjectProps = std::variant<SphereConfig, ConeConfig, PlaneConfig,
                                 CylinderConfig, std::monostate>;

/**
 * @brief Object configs
 */
struct ObjectConfig {
  ObjectType type = ObjectType::SPHERE;
  ObjectProps props;
  math::Material material;

  [[nodiscard]] static ObjectConfig
  loadConfig(const parsers::ConfigValue &config) noexcept;

  [[nodiscard]] static ObjectType
  stringToObjectType(const std::string &objectType) noexcept;

  [[nodiscard]] static ObjectProps
  createDefaultObjectProps(ObjectType type) noexcept;

  static void setObjectProps(ObjectConfig &object,
                             const parsers::ConfigValue &propsConfig) noexcept;
};
} // namespace temprenderer::core::config
