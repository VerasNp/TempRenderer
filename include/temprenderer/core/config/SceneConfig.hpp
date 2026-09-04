#pragma once
#include "core/math/Materials.hpp"
#include "kwp/Point3.hpp"
#include <toml++/toml.hpp>

#include <string>
#include <variant>
#include <vector>

namespace temprenderer::core::config {

enum class LightType { POINT, AMBIENT };

struct LightConfig {
  LightType type = LightType::POINT;
  kwp::Point3 position;
  math::ColorF color;
  kwp::Scalar intensity;
};

enum class ObjectType { SPHERE, OBJECT };

struct SphereConfig {
  kwp::Point3 center{0, 0, 0};
  kwp::Scalar radius = 1.0F;
};

struct ObjectConfig {
  ObjectType type = ObjectType::OBJECT;
  std::variant<SphereConfig> props;
  math::Material material;
};

struct SceneConfig {
  std::vector<LightConfig> lights;
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
};
} // namespace temprenderer::core::config
