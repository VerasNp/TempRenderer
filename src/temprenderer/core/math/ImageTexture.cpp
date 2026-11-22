#include "temprenderer/core/math/ImageTexture.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <string>

namespace temprenderer::core::math {

ImageTexture::ImageTexture(const std::filesystem::path &path, float scale,
                           ImageProjection projection)
    : scale_(scale), projection_(projection) {
  if (!std::isfinite(scale_) || scale_ <= 0.0F) {
    throw std::runtime_error("Image texture scale must be positive and finite");
  }
  load(path);
}

ColorF ImageTexture::sample(const TextureContext &context) const noexcept {
  constexpr float spherePreviewRadius = 0.435F;
  const bool spherePreview = projection_ == ImageProjection::SPHERE_PREVIEW;
  const float u = spherePreview
                      ? 0.5F + std::clamp(context.normal.x, -1.0F, 1.0F) *
                                   spherePreviewRadius
                      : context.uv.u * scale_;
  const float v = spherePreview
                      ? 0.5F - std::clamp(context.normal.y, -1.0F, 1.0F) *
                                   spherePreviewRadius
                      : context.uv.v * scale_;
  const bool repeatU = !spherePreview && context.repeatU;
  const bool repeatV = !spherePreview && context.repeatV;
  const float mappedU = repeatU ? u - std::floor(u) : std::clamp(u, 0.0F, 1.0F);
  const float mappedV = repeatV ? v - std::floor(v) : std::clamp(v, 0.0F, 1.0F);
  const float x = mappedU * static_cast<float>(width_) - 0.5F;
  const float y = mappedV * static_cast<float>(height_) - 0.5F;
  const int x0 = static_cast<int>(std::floor(x));
  const int y0 = static_cast<int>(std::floor(y));
  const float tx = x - static_cast<float>(x0);
  const float ty = y - static_cast<float>(y0);
  const ColorF top = mix(pixel(mapX(x0, repeatU), mapY(y0, repeatV)),
                         pixel(mapX(x0 + 1, repeatU), mapY(y0, repeatV)), tx);
  const ColorF bottom =
      mix(pixel(mapX(x0, repeatU), mapY(y0 + 1, repeatV)),
          pixel(mapX(x0 + 1, repeatU), mapY(y0 + 1, repeatV)), tx);
  return mix(top, bottom, ty);
}

std::string ImageTexture::readToken(std::istream &input) {
  std::string token;
  while (input >> token) {
    if (!token.empty() && token.front() == '#') {
      std::string remainder;
      std::getline(input, remainder);
      continue;
    }
    return token;
  }
  throw std::runtime_error("Unexpected end of PPM image");
}

void ImageTexture::load(const std::filesystem::path &path) {
  std::ifstream input(path, std::ios::binary);
  if (!input) {
    throw std::runtime_error("Unable to open image texture: " + path.string());
  }
  const std::string magic = readToken(input);
  if (magic != "P3" && magic != "P6") {
    throw std::runtime_error("Unsupported image format for texture: " +
                             path.string());
  }
  const int width = std::stoi(readToken(input));
  const int height = std::stoi(readToken(input));
  const int maxValue = std::stoi(readToken(input));
  if (width <= 0 || height <= 0 || maxValue <= 0 || maxValue > 65535 ||
      static_cast<std::uint64_t>(width) * static_cast<std::uint64_t>(height) >
          100000000ULL) {
    throw std::runtime_error("Invalid PPM dimensions or channel range: " +
                             path.string());
  }
  width_ = width;
  height_ = height;
  pixels_.reserve(static_cast<std::size_t>(width_) *
                  static_cast<std::size_t>(height_));
  if (magic == "P3") {
    for (int i = 0; i < width_ * height_; ++i) {
      const float r = static_cast<float>(std::stoi(readToken(input))) /
                      static_cast<float>(maxValue);
      const float g = static_cast<float>(std::stoi(readToken(input))) /
                      static_cast<float>(maxValue);
      const float b = static_cast<float>(std::stoi(readToken(input))) /
                      static_cast<float>(maxValue);
      pixels_.push_back({r, g, b});
    }
    return;
  }

  char separator = '\0';
  input.get(separator);
  if (separator == '\r' && input.peek() == '\n') {
    input.get(separator);
  }
  if (!std::isspace(static_cast<unsigned char>(separator))) {
    throw std::runtime_error("Invalid PPM binary header: " + path.string());
  }
  const auto readChannel = [&input, maxValue]() -> int {
    if (maxValue < 256) {
      return input.get();
    }
    const int high = input.get();
    const int low = input.get();
    if (high < 0 || low < 0) {
      return -1;
    }
    return (high << 8) | low;
  };
  for (int i = 0; i < width_ * height_; ++i) {
    const int r = readChannel();
    const int g = readChannel();
    const int b = readChannel();
    if (r < 0 || g < 0 || b < 0) {
      throw std::runtime_error("Truncated PPM image: " + path.string());
    }
    pixels_.push_back({static_cast<float>(r) / static_cast<float>(maxValue),
                       static_cast<float>(g) / static_cast<float>(maxValue),
                       static_cast<float>(b) / static_cast<float>(maxValue)});
  }
}

int ImageTexture::mapX(int x, bool repeat) const noexcept {
  if (!repeat) {
    return std::clamp(x, 0, width_ - 1);
  }
  const int wrapped = x % width_;
  return wrapped < 0 ? wrapped + width_ : wrapped;
}

int ImageTexture::mapY(int y, bool repeat) const noexcept {
  if (!repeat) {
    return std::clamp(y, 0, height_ - 1);
  }
  const int wrapped = y % height_;
  return wrapped < 0 ? wrapped + height_ : wrapped;
}

ColorF ImageTexture::pixel(int x, int y) const noexcept {
  return pixels_[static_cast<std::size_t>(y) *
                     static_cast<std::size_t>(width_) +
                 static_cast<std::size_t>(x)];
}

ColorF ImageTexture::mix(const ColorF &a, const ColorF &b,
                         float amount) noexcept {
  return a * (1.0F - amount) + b * amount;
}

} // namespace temprenderer::core::math
