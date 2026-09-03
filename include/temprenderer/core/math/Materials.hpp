#pragma once
#include <cstdint>

namespace temprenderer::core::math {

struct Color {
  std::uint8_t r, g, b;
};

struct Material {
  virtual ~Material() = default;
  virtual Color getColor() const = 0;
};

struct DiffuseOnlyMaterial : Material {
  Color kd;

  explicit DiffuseOnlyMaterial(Color kd_) : kd(kd_) {}
  Color getColor() const override { return kd; };
};

struct DiffuseSpecularMaterial : Material {
  Color kd;
  Color ks;
  std::uint16_t alpha;

  DiffuseSpecularMaterial(Color kd_, Color ks_, std::uint16_t alpha_)
      : kd(kd_), ks(ks_), alpha(alpha_) {}
  Color getColor() const override { return kd; };
};
} // namespace temprenderer::core::math
