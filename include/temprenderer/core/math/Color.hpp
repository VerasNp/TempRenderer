#pragma once

namespace temprenderer::core::math {
struct Color {
  float r = 0.0f, g = 0.0f, b = 0.0f, a = 1.0f;

  constexpr Color operator*(float scalar) const {
    return {.r = r * scalar, .g = g * scalar, .b = b * scalar, .a = a};
  }
  constexpr Color operator+(const Color &other) const {
    return {.r = r + other.r, .g = g + other.g, .b = b + other.b, .a = a};
  }
};
constexpr Color operator*(float scalar, const Color &c) { return c * scalar; }
} // namespace temprenderer::core::math
