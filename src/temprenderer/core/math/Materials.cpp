#include "temprenderer/core/math/Materials.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>

namespace temprenderer::core::math {
namespace {

float hash(int x, int y, int seed) noexcept {
  std::uint32_t value = static_cast<std::uint32_t>(x) * 0x8da6b343U;
  value ^= static_cast<std::uint32_t>(y) * 0xd8163841U;
  value ^= static_cast<std::uint32_t>(seed) * 0xcb1ab31fU;
  value ^= value >> 16U;
  value *= 0x7feb352dU;
  value ^= value >> 15U;
  value *= 0x846ca68bU;
  value ^= value >> 16U;
  return static_cast<float>(value & 0x00ffffffU) / 16777215.0F;
}

float hash(int x, int y, int z, int seed) noexcept {
  std::uint32_t value = static_cast<std::uint32_t>(x) * 0x8da6b343U;
  value ^= static_cast<std::uint32_t>(y) * 0xd8163841U;
  value ^= static_cast<std::uint32_t>(z) * 0xcb1ab31fU;
  value ^= static_cast<std::uint32_t>(seed) * 0x165667b1U;
  value ^= value >> 16U;
  value *= 0x7feb352dU;
  value ^= value >> 15U;
  value *= 0x846ca68bU;
  value ^= value >> 16U;
  return static_cast<float>(value & 0x00ffffffU) / 16777215.0F;
}

float valueNoise(float x, float y, int seed) noexcept {
  const int x0 = static_cast<int>(std::floor(x));
  const int y0 = static_cast<int>(std::floor(y));
  const float tx = x - static_cast<float>(x0);
  const float ty = y - static_cast<float>(y0);
  const float sx = tx * tx * (3.0F - 2.0F * tx);
  const float sy = ty * ty * (3.0F - 2.0F * ty);
  const float lower =
      hash(x0, y0, seed) * (1.0F - sx) + hash(x0 + 1, y0, seed) * sx;
  const float upper =
      hash(x0, y0 + 1, seed) * (1.0F - sx) + hash(x0 + 1, y0 + 1, seed) * sx;
  return lower * (1.0F - sy) + upper * sy;
}

float valueNoise(float x, float y, float z, int seed) noexcept {
  const int x0 = static_cast<int>(std::floor(x));
  const int y0 = static_cast<int>(std::floor(y));
  const int z0 = static_cast<int>(std::floor(z));
  const float tx = x - static_cast<float>(x0);
  const float ty = y - static_cast<float>(y0);
  const float tz = z - static_cast<float>(z0);
  const float sx = tx * tx * (3.0F - 2.0F * tx);
  const float sy = ty * ty * (3.0F - 2.0F * ty);
  const float sz = tz * tz * (3.0F - 2.0F * tz);
  const float c000 = hash(x0, y0, z0, seed);
  const float c100 = hash(x0 + 1, y0, z0, seed);
  const float c010 = hash(x0, y0 + 1, z0, seed);
  const float c110 = hash(x0 + 1, y0 + 1, z0, seed);
  const float c001 = hash(x0, y0, z0 + 1, seed);
  const float c101 = hash(x0 + 1, y0, z0 + 1, seed);
  const float c011 = hash(x0, y0 + 1, z0 + 1, seed);
  const float c111 = hash(x0 + 1, y0 + 1, z0 + 1, seed);
  const float lower = (c000 * (1.0F - sx) + c100 * sx) * (1.0F - sy) +
                      (c010 * (1.0F - sx) + c110 * sx) * sy;
  const float upper = (c001 * (1.0F - sx) + c101 * sx) * (1.0F - sy) +
                      (c011 * (1.0F - sx) + c111 * sx) * sy;
  return lower * (1.0F - sz) + upper * sz;
}

float smoothstep(float edge0, float edge1, float value) noexcept {
  const float t = std::clamp((value - edge0) / (edge1 - edge0), 0.0F, 1.0F);
  return t * t * (3.0F - 2.0F * t);
}

ColorF mix(const ColorF &a, const ColorF &b, float amount) noexcept {
  return a * (1.0F - amount) + b * amount;
}

kwp::Vec3 texturePosition(const TextureContext &context) noexcept {
  const kwp::Vec3 point{context.localPosition.x, context.localPosition.y,
                        context.localPosition.z};
  if (point.length() > 1.0e-5F) {
    return point.normalize();
  }
  constexpr float pi = 3.14159265358979323846F;
  const float latitude = (0.5F - context.uv.v) * pi;
  const float longitude = (context.uv.u - 0.5F) * 2.0F * pi;
  const float cosLatitude = std::cos(latitude);
  return {cosLatitude * std::cos(longitude), std::sin(latitude),
          cosLatitude * std::sin(longitude)};
}

} // namespace

WoodTexture::WoodTexture(ColorF baseColor, ColorF ringColor, float scale,
                         float turbulence, float plankWidth, float plankLength,
                         float offsetX, float offsetZ) noexcept
    : baseColor_(baseColor), ringColor_(ringColor), scale_(scale),
      turbulence_(turbulence), plankWidth_(plankWidth),
      plankLength_(plankLength), offsetX_(offsetX), offsetZ_(offsetZ) {}

ColorF WoodTexture::sample(const TextureContext &context) const noexcept {
  constexpr float tau = 6.2831853F;
  const kwp::Point3 &point = context.worldPosition;
  const float plankWidth = std::max(plankWidth_, 0.5F);
  const float plankLength = std::max(plankLength_, 0.5F);
  const float joint =
      std::min(0.75F, std::min(plankWidth, plankLength) * 0.025F);
  const float sampleX = point.x + offsetX_;
  const float sampleZ = point.z + offsetZ_;
  const float plankX = std::floor(sampleX / plankWidth);
  const int ix = static_cast<int>(plankX);
  const float stagger = hash(ix, 0, 211) * plankLength * 0.65F;
  const float plankZ = std::floor((sampleZ + stagger) / plankLength);
  const int iz = static_cast<int>(plankZ);
  const float localX = sampleX - plankX * plankWidth;
  const float localZ = sampleZ + stagger - plankZ * plankLength;

  if (localX < joint || localX > plankWidth - joint || localZ < joint ||
      localZ > plankLength - joint) {
    return {0.16F, 0.07F, 0.025F};
  }

  const float u = (localX - joint) / (plankWidth - 2.0F * joint);
  const float v = (localZ - joint) / (plankLength - 2.0F * joint);
  const float grainScale = std::clamp(scale_, 0.25F, 64.0F);
  const float turbulence = std::clamp(turbulence_, 0.0F, 2.0F);
  const float boardVariation = hash(ix, iz, 17);
  const float broadNoise = valueNoise(sampleX * 0.055F, sampleZ * 0.035F, 31);
  const float fineNoise = valueNoise(sampleX * 0.31F, sampleZ * 0.16F, 73);
  const float wave = std::sin(v * tau * 1.7F + boardVariation * 6.0F);
  const float phase = u * grainScale * tau +
                      turbulence * (wave * 0.42F + (broadNoise - 0.5F) * 2.4F);
  const float ring =
      std::pow(std::max(0.0F, 0.5F + 0.5F * std::cos(phase)), 9.0F);
  const float grainVariation = 0.82F + broadNoise * 0.2F + fineNoise * 0.08F;
  const float colorMix = std::clamp(
      0.08F + ring * 0.48F + (fineNoise - 0.5F) * 0.08F, 0.0F, 0.72F);
  ColorF color = mix(baseColor_, ringColor_, colorMix);
  color = color * (grainVariation * (0.9F + boardVariation * 0.2F));

  if (hash(ix, iz, 101) > 0.62F) {
    const float knotU = 0.12F + hash(ix, iz, 109) * 0.76F;
    const float knotV = 0.12F + hash(ix, iz, 127) * 0.76F;
    const float radiusU = 0.025F + hash(ix, iz, 137) * 0.025F;
    const float radiusV = 0.012F + hash(ix, iz, 149) * 0.012F;
    const float dx = (u - knotU) / radiusU;
    const float dz = (v - knotV) / radiusV;
    const float distance = std::sqrt(dx * dx + dz * dz);
    const float knotFade = 1.0F - smoothstep(0.72F, 1.6F, distance);
    const float knotRing = 0.5F + 0.5F * std::cos(distance * 9.0F);
    color = mix(color, ringColor_, knotFade * (0.42F + knotRing * 0.38F));
  }

  if (hash(ix, iz, 163) > 0.78F && (v < 0.18F || v > 0.82F)) {
    const float checkU = 0.15F + hash(ix, iz, 179) * 0.7F;
    const float checkWidth = 0.002F + hash(ix, iz, 191) * 0.003F;
    const float distance = std::abs(u - checkU);
    const float endFade = v < 0.18F ? 1.0F - smoothstep(0.02F, 0.18F, v)
                                    : smoothstep(0.82F, 0.98F, v);
    const float check =
        (1.0F - smoothstep(checkWidth, checkWidth * 2.5F, distance)) * endFade;
    color = mix(color, ringColor_ * 0.55F, check * 0.8F);
  }
  return color;
}

MetalTexture::MetalTexture(ColorF baseColor, ColorF scratchColor, float scale,
                           float mottling, float scratchDensity,
                           float pitStrength, float roughnessVariation,
                           float bumpStrength) noexcept
    : baseColor_(baseColor), scratchColor_(scratchColor), scale_(scale),
      mottling_(mottling), scratchDensity_(scratchDensity),
      pitStrength_(pitStrength), roughnessVariation_(roughnessVariation),
      bumpStrength_(bumpStrength) {}

ColorF MetalTexture::sample(const TextureContext &context) const noexcept {
  constexpr float tau = 6.2831853F;
  const float scale = std::clamp(scale_, 0.1F, 8.0F);
  const float u = context.uv.u;
  const float v = context.uv.v;
  const kwp::Vec3 position = texturePosition(context);
  const float cloud =
      valueNoise(position.x * 7.0F * scale, position.y * 7.0F * scale,
                 position.z * 7.0F * scale, 29);
  const float midScale =
      valueNoise(position.x * 23.0F * scale, position.y * 23.0F * scale,
                 position.z * 23.0F * scale, 43);
  const float fine =
      valueNoise(position.x * 96.0F * scale, position.y * 96.0F * scale,
                 position.z * 96.0F * scale, 71);
  const float scratchWarp = (midScale - 0.5F) * 0.28F;
  const float scratchPhase =
      v * tau * (24.0F + std::clamp(scratchDensity_, 0.0F, 2.0F) * 72.0F) +
      scratchWarp + (u - 0.5F) * 5.0F;
  const float scratchLine =
      std::pow(0.5F + 0.5F * std::cos(scratchPhase), 48.0F);
  const float scratches = scratchLine * std::clamp(scratchDensity_, 0.0F, 2.0F);
  const float pit = std::clamp((fine - 0.72F) * 3.2F, 0.0F, 0.65F) *
                    std::clamp(pitStrength_, 0.0F, 1.0F);
  const float cloudMix = std::clamp(
      (cloud - 0.5F) * std::clamp(mottling_, 0.0F, 1.5F) + 0.5F, 0.0F, 1.0F);
  const float midMix = (midScale - 0.5F) * 0.22F;
  ColorF color = mix(
      baseColor_, scratchColor_,
      std::clamp(cloudMix * 0.6F + midMix + scratches * 0.35F, 0.0F, 0.85F));
  color = mix(color, baseColor_ * 0.42F, pit);
  color = color * (0.84F + fine * 0.2F);
  return {std::clamp(color.r, 0.0F, 1.0F), std::clamp(color.g, 0.0F, 1.0F),
          std::clamp(color.b, 0.0F, 1.0F)};
}

float MetalTexture::sampleRoughness(const TextureContext &context,
                                    float roughness) const noexcept {
  const auto position = texturePosition(context);
  const float cloud =
      valueNoise(position.x * 11.0F * scale_, position.y * 11.0F * scale_,
                 position.z * 11.0F * scale_, 97);
  const float scale = std::clamp(scale_, 0.1F, 8.0F);
  const float fine =
      valueNoise(position.x * 83.0F * scale, position.y * 83.0F * scale,
                 position.z * 83.0F * scale, 101);
  const float variation = std::clamp(roughnessVariation_, 0.0F, 0.6F);
  return std::clamp(
      roughness + ((cloud - 0.5F) * 1.25F + (fine - 0.5F) * 0.35F) * variation,
      0.04F, 1.0F);
}

kwp::Vec3 MetalTexture::sampleNormal(const TextureContext &context,
                                     const kwp::Vec3 &normal) const noexcept {
  const float strength = std::clamp(bumpStrength_, 0.0F, 0.5F);
  if (strength == 0.0F) {
    return normal;
  }
  const auto position = texturePosition(context);
  constexpr float step = 0.006F;
  const float frequency = 28.0F * std::clamp(scale_, 0.1F, 8.0F);
  const float dx =
      valueNoise((position.x + step) * frequency, position.y * frequency,
                 position.z * frequency, 43) -
      valueNoise((position.x - step) * frequency, position.y * frequency,
                 position.z * frequency, 43);
  const float dy =
      valueNoise(position.x * frequency, (position.y + step) * frequency,
                 position.z * frequency, 43) -
      valueNoise(position.x * frequency, (position.y - step) * frequency,
                 position.z * frequency, 43);
  const float dz = valueNoise(position.x * frequency, position.y * frequency,
                              (position.z + step) * frequency, 43) -
                   valueNoise(position.x * frequency, position.y * frequency,
                              (position.z - step) * frequency, 43);
  const kwp::Vec3 gradient{dx, dy, dz};
  const kwp::Vec3 tangentGradient =
      gradient - normal * kwp::dot(gradient, normal);
  const kwp::Vec3 perturbed = normal - tangentGradient * (strength * 1.8F);
  return kwp::dot(perturbed, normal) > 0.0F ? perturbed.normalize() : normal;
}

ColorF evaluateMetalSpecular(const ColorF &f0, float nDotL, float nDotV,
                             float nDotH, float vDotH,
                             float roughness) noexcept {
  constexpr float pi = 3.14159265358979323846F;
  nDotL = std::clamp(nDotL, 0.0F, 1.0F);
  nDotV = std::clamp(nDotV, 0.0F, 1.0F);
  nDotH = std::clamp(nDotH, 0.0F, 1.0F);
  vDotH = std::clamp(vDotH, 0.0F, 1.0F);
  roughness = std::clamp(roughness, 0.04F, 1.0F);
  if (nDotL <= 0.0F || nDotV <= 0.0F) {
    return {0.0F, 0.0F, 0.0F};
  }

  const float alpha = roughness * roughness;
  const float alphaSquared = alpha * alpha;
  const float denominator = nDotH * nDotH * (alphaSquared - 1.0F) + 1.0F;
  const float distribution = alphaSquared / (pi * denominator * denominator);
  const float k = (roughness + 1.0F) * (roughness + 1.0F) / 8.0F;
  const float geometryL = nDotL / (nDotL * (1.0F - k) + k);
  const float geometryV = nDotV / (nDotV * (1.0F - k) + k);
  const float fresnelWeight = std::pow(1.0F - vDotH, 5.0F);
  const ColorF fresnel{f0.r + (1.0F - f0.r) * fresnelWeight,
                       f0.g + (1.0F - f0.g) * fresnelWeight,
                       f0.b + (1.0F - f0.b) * fresnelWeight};
  return fresnel * (distribution * geometryL * geometryV /
                    std::max(4.0F * nDotL * nDotV, 1.0e-5F));
}

} // namespace temprenderer::core::math
