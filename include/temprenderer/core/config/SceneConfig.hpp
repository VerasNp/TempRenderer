#pragma once
#include "core/math/Materials.hpp"
#include "kwp/Point3.hpp"
#include <toml++/toml.hpp>

#include <string>
#include <variant>
#include <vector>

namespace temprenderer::core::config {
struct LightConfig {
  kwp::Point3 position;
  math::Color color;
  kwp::Scalar intensity;
};

enum class ObjectType { SPHERE, OBJECT };

enum class MaterialType { DIFFUSE_ONLY, DIFFUSE_SPECULAR };

struct SphereConfig {
  kwp::Point3 center{0, 0, 0};
  kwp::Scalar radius = 1.0F;
};

struct DiffuseOnlyMaterialConfig {
  math::Color kd;
};

struct DiffuseSpecularMaterialConfig {
  math::Color kd;
  math::Color ks;
  std::uint16_t alpha = 0;
};

struct MaterialConfig {
  MaterialType type = MaterialType::DIFFUSE_ONLY;
  std::variant<DiffuseOnlyMaterialConfig, DiffuseSpecularMaterialConfig> props;
};

struct ObjectConfig {
  ObjectType type = ObjectType::OBJECT;
  std::variant<SphereConfig> props;
  MaterialConfig material;
};

struct SceneConfig {
  LightConfig light;
  std::vector<ObjectConfig> objects;

  /**
   * @brief TODO
   *
   * @param table
   * @return
   */
  static SceneConfig loadSceneConfig(const toml::table &table);
  /**
   * @brief TODO
   *
   * @param objectType
   * @return
   */
  [[nodiscard]] static std::string
  objectTypeToString(const ObjectType objectType) noexcept;
  /**
   * @brief TODO
   *
   * @param materialType
   * @return
   */
  [[nodiscard]] static std::string
  materialTypeToString(const MaterialType materialType) noexcept;
};
} // namespace temprenderer::core::config
