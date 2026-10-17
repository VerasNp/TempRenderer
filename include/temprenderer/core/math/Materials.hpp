#pragma once
#include "kwp/Point3.hpp"
#include "kwp/math.hpp"

#include <cmath>
#include <cstdint>
#include <memory>
#include <optional>
#include <utility>

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

struct TextureCoordinates {
  float u = 0.0F;
  float v = 0.0F;
};

struct TextureContext {
  kwp::Point3 worldPosition{0.0F, 0.0F, 0.0F};
  kwp::Point3 localPosition{0.0F, 0.0F, 0.0F};
  kwp::Vec3 normal{0.0F, 1.0F, 0.0F};
  TextureCoordinates uv;
  bool repeatU = true;
  bool repeatV = true;
};

[[nodiscard]] inline std::pair<kwp::Vec3, kwp::Vec3>
makeTextureFrame(const kwp::Vec3 &axis) noexcept {
  const kwp::Vec3 reference = std::abs(axis.y) < 0.9F
                                  ? kwp::Vec3{0.0F, 1.0F, 0.0F}
                                  : kwp::Vec3{1.0F, 0.0F, 0.0F};
  const kwp::Vec3 tangent = kwp::cross(reference, axis).normalize();
  return {tangent, kwp::cross(axis, tangent).normalize()};
}

struct Texture {
  virtual ~Texture() = default;

  [[nodiscard]] virtual ColorF
  sample(const TextureContext &context) const noexcept = 0;

  [[nodiscard]] virtual float sampleRoughness(const TextureContext &,
                                              float roughness) const noexcept {
    return roughness;
  }

  [[nodiscard]] virtual kwp::Vec3
  sampleNormal(const TextureContext &, const kwp::Vec3 &normal) const noexcept {
    return normal;
  }

  [[nodiscard]] ColorF sample(const kwp::Point3 &point) const noexcept {
    TextureContext context;
    context.worldPosition = point;
    context.localPosition = point;
    return sample(context);
  }
};

struct SolidColorTexture final : Texture {
  using Texture::sample;

  explicit SolidColorTexture(ColorF color = {1.0F, 1.0F, 1.0F}) noexcept
      : color_(color) {}

  [[nodiscard]] ColorF sample(const TextureContext &) const noexcept override {
    return color_;
  }

private:
  ColorF color_;
};

struct WoodTexture final : Texture {
  using Texture::sample;

  explicit WoodTexture(ColorF baseColor = {0.72F, 0.46F, 0.22F},
                       ColorF ringColor = {0.38F, 0.20F, 0.08F},
                       float scale = 8.0F, float turbulence = 0.65F,
                       float plankWidth = 35.0F, float plankLength = 120.0F,
                       float offsetX = 0.0F, float offsetZ = 0.0F) noexcept;

  [[nodiscard]] ColorF
  sample(const TextureContext &context) const noexcept override;

private:
  ColorF baseColor_;
  ColorF ringColor_;
  float scale_;
  float turbulence_;
  float plankWidth_;
  float plankLength_;
  float offsetX_;
  float offsetZ_;
};

struct MetalTexture final : Texture {
  using Texture::sample;

  explicit MetalTexture(ColorF baseColor = {0.43F, 0.43F, 0.43F},
                        ColorF scratchColor = {0.68F, 0.68F, 0.68F},
                        float scale = 1.0F, float mottling = 0.75F,
                        float scratchDensity = 0.55F, float pitStrength = 0.2F,
                        float roughnessVariation = 0.2F,
                        float bumpStrength = 0.12F) noexcept;

  [[nodiscard]] ColorF
  sample(const TextureContext &context) const noexcept override;

  [[nodiscard]] float sampleRoughness(const TextureContext &context,
                                      float roughness) const noexcept override;

  [[nodiscard]] kwp::Vec3
  sampleNormal(const TextureContext &context,
               const kwp::Vec3 &normal) const noexcept override;

private:
  ColorF baseColor_;
  ColorF scratchColor_;
  float scale_;
  float mottling_;
  float scratchDensity_;
  float pitStrength_;
  float roughnessVariation_;
  float bumpStrength_;
};

[[nodiscard]] ColorF evaluateMetalSpecular(const ColorF &f0, float nDotL,
                                           float nDotV, float nDotH,
                                           float vDotH,
                                           float roughness) noexcept;

struct Material {
  std::shared_ptr<const Texture> texture;
  ColorF kd = ColorF{0, 0, 0};
  std::optional<ColorF> ks;
  std::optional<ColorF> ka;
  std::optional<float> alpha;
  float metallic = 0.0F;
  float roughness = 0.5F;

  [[nodiscard]] ColorF
  sampleColor(const TextureContext &context) const noexcept {
    if (texture) {
      return texture->sample(context);
    }
    return kd;
  }

  [[nodiscard]] float
  sampleRoughness(const TextureContext &context) const noexcept {
    return texture ? texture->sampleRoughness(context, roughness) : roughness;
  }

  [[nodiscard]] kwp::Vec3 sampleNormal(const TextureContext &context,
                                       const kwp::Vec3 &normal) const noexcept {
    return texture ? texture->sampleNormal(context, normal) : normal;
  }

  [[nodiscard]] ColorF sampleColor(const kwp::Point3 &point) const noexcept {
    TextureContext context;
    context.worldPosition = point;
    context.localPosition = point;
    return sampleColor(context);
  }
};
} // namespace temprenderer::core::math
