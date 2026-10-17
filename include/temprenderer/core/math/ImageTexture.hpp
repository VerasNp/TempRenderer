#pragma once

#include "Materials.hpp"

#include <filesystem>
#include <istream>
#include <string>
#include <vector>

namespace temprenderer::core::math {

enum class ImageProjection { UV, SPHERE_PREVIEW };

struct ImageTexture final : Texture {
  using Texture::sample;

  explicit ImageTexture(const std::filesystem::path &path, float scale = 1.0F,
                        ImageProjection projection = ImageProjection::UV);

  [[nodiscard]] ColorF
  sample(const TextureContext &context) const noexcept override;

private:
  void load(const std::filesystem::path &path);

  [[nodiscard]] static std::string readToken(std::istream &input);
  [[nodiscard]] int mapX(int x, bool repeat) const noexcept;
  [[nodiscard]] int mapY(int y, bool repeat) const noexcept;
  [[nodiscard]] ColorF pixel(int x, int y) const noexcept;
  [[nodiscard]] static ColorF mix(const ColorF &a, const ColorF &b,
                                  float amount) noexcept;

  int width_ = 0;
  int height_ = 0;
  float scale_;
  ImageProjection projection_;
  std::vector<ColorF> pixels_;
};

} // namespace temprenderer::core::math
