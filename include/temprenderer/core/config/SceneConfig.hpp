#pragma once
#include "core/math/Color.hpp"
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

struct SphereConfig {
  kwp::Point3 center{0, 0, 0};
  kwp::Scalar radius = 1.0F;
};

struct MaterialConfig {
  math::Color kd;
  math::Color ks;
  std::uint16_t alpha = 0;
};

struct ObjectConfig {
  ObjectType type = ObjectType::OBJECT;
  std::variant<SphereConfig> props;
  MaterialConfig material;
};

struct SceneConfig {
  LightConfig light;
  std::vector<ObjectConfig> objects;

  static SceneConfig loadSceneConfig(const toml::table &table);
};
} // namespace temprenderer::core::config
