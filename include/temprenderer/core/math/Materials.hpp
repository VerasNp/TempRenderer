#pragma once
#include "kwp/math.hpp"

#include <cstdint>
#include <optional>

namespace temprenderer::core::math {

struct ColorF {
  float r, g, b;

  ColorF operator*(float scalar) const noexcept {
    return {r * scalar, g * scalar, b * scalar};
  }

  ColorF operator+(const ColorF &other) const noexcept {
    return {r + other.r, g + other.g, b + other.b};
  }

  ColorF operator*(const ColorF &other) const noexcept {
    return {r * other.r, g * other.g, b * other.b};
  }

  ColorF &operator+=(const ColorF &other) noexcept {
    r += other.r;
    g += other.g;
    b += other.b;
    return *this;
  }
};

struct Color {
  std::uint8_t r, g, b;

  [[nodiscard]] ColorF toFloat() const noexcept {
    return {r / 255.0f, g / 255.0f, b / 255.0f};
  }

  [[nodiscard]] static Color fromFloat(const ColorF &c) noexcept {
    return {
        static_cast<std::uint8_t>(kwp::clamp(c.r, 0.0f, 1.0f) * 255.0f),
        static_cast<std::uint8_t>(kwp::clamp(c.g, 0.0f, 1.0f) * 255.0f),
        static_cast<std::uint8_t>(kwp::clamp(c.b, 0.0f, 1.0f) * 255.0f),
    };
  }
};

struct Material {
  ColorF kd = ColorF{0, 0, 0};
  std::optional<ColorF> ks;
  std::optional<ColorF> ka;
  std::optional<std::uint16_t> alpha;
};

// struct DiffuseOnlyMaterial : Material {
//   ColorF kd;
//
//   explicit DiffuseOnlyMaterial(ColorF kd_) : kd(kd_) {}
//   ColorF getColor() const override { return kd; };
// };
//
// struct DiffuseSpecularMaterial : Material {
//   ColorF kd;
//   ColorF ks;
//   std::uint16_t alpha;
//
//   DiffuseSpecularMaterial(ColorF kd_, ColorF ks_, std::uint16_t alpha_)
//       : kd(kd_), ks(ks_), alpha(alpha_) {}
//   ColorF getColor() const override { return kd; };
// };
} // namespace temprenderer::core::math
