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
    return {r / 255.0F, g / 255.0F, b / 255.0F};
  }

  [[nodiscard]] static Color fromFloat(const ColorF &c) noexcept {
    return {
        static_cast<std::uint8_t>(kwp::clamp(c.r, 0.0F, 1.0F) * 255.0F),
        static_cast<std::uint8_t>(kwp::clamp(c.g, 0.0F, 1.0F) * 255.0F),
        static_cast<std::uint8_t>(kwp::clamp(c.b, 0.0F, 1.0F) * 255.0F),
    };
  }
};

struct Material {
  ColorF kd = ColorF{0, 0, 0};
  std::optional<ColorF> ks;
  std::optional<ColorF> ka;
  std::optional<float> alpha;
};
} // namespace temprenderer::core::math
