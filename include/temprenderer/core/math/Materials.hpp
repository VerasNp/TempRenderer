#pragma once
#include "kwp/Point3.hpp"
#include "kwp/math.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <memory>
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

struct Texture {
  virtual ~Texture() = default;

  [[nodiscard]] virtual ColorF
  sample(const kwp::Point3 &point) const noexcept = 0;
};

struct SolidColorTexture final : Texture {
  explicit SolidColorTexture(ColorF color = {1.0F, 1.0F, 1.0F}) noexcept
      : color_(color) {}

  [[nodiscard]] ColorF sample(const kwp::Point3 &) const noexcept override {
    return color_;
  }

private:
  ColorF color_;
};

struct WoodTexture final : Texture {
  explicit WoodTexture(ColorF baseColor = {0.72F, 0.46F, 0.22F},
                       ColorF ringColor = {0.38F, 0.20F, 0.08F},
                       float scale = 8.0F, float turbulence = 0.65F) noexcept
      : baseColor_(baseColor), ringColor_(ringColor), scale_(scale),
        turbulence_(turbulence) {}

  [[nodiscard]] ColorF
  sample(const kwp::Point3 &point) const noexcept override {
    constexpr float plankWidth = 35.0F;
    constexpr float plankLength = 120.0F;
    constexpr float joint = 0.75F;

    const float fx = std::floor(point.x / plankWidth);
    const int ix = static_cast<int>(fx);
    const float stagger = hash(ix, 0, 211) * plankLength * 0.65F;
    const float fz = std::floor((point.z + stagger) / plankLength);
    const int iz = static_cast<int>(fz);

    const float localX = point.x - fx * plankWidth;
    const float localZ = point.z + stagger - fz * plankLength;

    const bool border = localX < joint || localX > plankWidth - joint ||
                        localZ < joint || localZ > plankLength - joint;
    if (border) {
      return {0.16F, 0.07F, 0.025F};
    }

    const float u = (localX - joint) / (plankWidth - 2.0F * joint);
    const float v = (localZ - joint) / (plankLength - 2.0F * joint);
    const float grainScale = std::clamp(scale_, 0.25F, 64.0F);
    const float turbulence = std::clamp(turbulence_, 0.0F, 2.0F);
    const float boardVariation = hash(ix, iz, 17);
    const float broadNoise = valueNoise(point.x * 0.055F, point.z * 0.035F, 31);
    const float fineNoise = valueNoise(point.x * 0.31F, point.z * 0.16F, 73);
    const float wave = std::sin(v * 6.2831853F * 1.7F + boardVariation * 6.0F);
    const float phase =
        u * grainScale * 6.2831853F +
        turbulence * (wave * 0.42F + (broadNoise - 0.5F) * 2.4F);
    const float ring =
        std::pow(std::max(0.0F, 0.5F + 0.5F * std::cos(phase)), 9.0F);
    const float grainVariation = 0.82F + broadNoise * 0.2F + fineNoise * 0.08F;
    const float colorMix = std::clamp(
        0.08F + ring * 0.48F + (fineNoise - 0.5F) * 0.08F, 0.0F, 0.72F);
    ColorF color = mix(baseColor_, ringColor_, colorMix);
    const float boardTint = 0.9F + boardVariation * 0.2F;
    color = color * (grainVariation * boardTint);

    const float knotPresence = hash(ix, iz, 101);
    if (knotPresence > 0.62F) {
      const float knotU = 0.12F + hash(ix, iz, 109) * 0.76F;
      const float knotV = 0.12F + hash(ix, iz, 127) * 0.76F;
      const float knotRadiusU = 0.025F + hash(ix, iz, 137) * 0.025F;
      const float knotRadiusV = 0.012F + hash(ix, iz, 149) * 0.012F;
      const float dx = (u - knotU) / knotRadiusU;
      const float dz = (v - knotV) / knotRadiusV;
      const float knotDistance = std::sqrt(dx * dx + dz * dz);
      const float knotFade = 1.0F - smoothstep(0.72F, 1.6F, knotDistance);
      const float knotRing = 0.5F + 0.5F * std::cos(knotDistance * 9.0F);
      color = mix(color, ringColor_, knotFade * (0.42F + knotRing * 0.38F));
    }

    const float checkPresence = hash(ix, iz, 163);
    if (checkPresence > 0.78F && (v < 0.18F || v > 0.82F)) {
      const float checkU = 0.15F + hash(ix, iz, 179) * 0.7F;
      const float checkWidth = 0.002F + hash(ix, iz, 191) * 0.003F;
      const float distance = std::abs(u - checkU);
      const float endFade = v < 0.18F ? 1.0F - smoothstep(0.02F, 0.18F, v)
                                      : smoothstep(0.82F, 0.98F, v);
      const float check =
          (1.0F - smoothstep(checkWidth, checkWidth * 2.5F, distance)) *
          endFade;
      color = mix(color, ringColor_ * 0.55F, check * 0.8F);
    }

    return color;
  }

private:
  [[nodiscard]] static float hash(int x, int z, int seed) noexcept {
    std::uint32_t value = static_cast<std::uint32_t>(x) * 0x8da6b343U;
    value ^= static_cast<std::uint32_t>(z) * 0xd8163841U;
    value ^= static_cast<std::uint32_t>(seed) * 0xcb1ab31fU;
    value ^= value >> 16U;
    value *= 0x7feb352dU;
    value ^= value >> 15U;
    value *= 0x846ca68bU;
    value ^= value >> 16U;
    return static_cast<float>(value & 0x00ffffffU) / 16777215.0F;
  }

  [[nodiscard]] static float valueNoise(float x, float z, int seed) noexcept {
    const int x0 = static_cast<int>(std::floor(x));
    const int z0 = static_cast<int>(std::floor(z));
    const float tx = x - static_cast<float>(x0);
    const float tz = z - static_cast<float>(z0);
    const float sx = tx * tx * (3.0F - 2.0F * tx);
    const float sz = tz * tz * (3.0F - 2.0F * tz);
    const float lower =
        hash(x0, z0, seed) * (1.0F - sx) + hash(x0 + 1, z0, seed) * sx;
    const float upper =
        hash(x0, z0 + 1, seed) * (1.0F - sx) + hash(x0 + 1, z0 + 1, seed) * sx;
    return lower * (1.0F - sz) + upper * sz;
  }

  [[nodiscard]] static float smoothstep(float edge0, float edge1,
                                        float value) noexcept {
    const float t = std::clamp((value - edge0) / (edge1 - edge0), 0.0F, 1.0F);
    return t * t * (3.0F - 2.0F * t);
  }

  [[nodiscard]] static ColorF mix(const ColorF &a, const ColorF &b,
                                  float amount) noexcept {
    return a * (1.0F - amount) + b * amount;
  }

  ColorF baseColor_;
  ColorF ringColor_;
  float scale_;
  float turbulence_;
};

struct Material {
  std::shared_ptr<const Texture> texture;
  ColorF kd = ColorF{0, 0, 0};
  std::optional<ColorF> ks;
  std::optional<ColorF> ka;
  std::optional<float> alpha;

  [[nodiscard]] ColorF sampleColor(const kwp::Point3 &point) const noexcept {
    if (texture) {
      return texture->sample(point);
    }
    return kd;
  }
};
} // namespace temprenderer::core::math
