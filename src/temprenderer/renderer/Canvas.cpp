#include "temprenderer/renderer/Canvas.hpp"
namespace temprenderer::renderer {
Canvas::Canvas(unsigned int renderingWidth,
               unsigned int renderingHeight) noexcept
    : renderingWidth_(renderingWidth), renderingHeight_(renderingHeight),
      pixels_(static_cast<std::size_t>(renderingWidth) *
              static_cast<std::size_t>(renderingHeight)) {}

void Canvas::set(unsigned int col, unsigned int row,
                 core::math::Color color) noexcept {
  this->pixels_[index(col, row)] = color;
}

core::math::Color Canvas::get(unsigned int col,
                              unsigned int row) const noexcept {
  return this->pixels_[index(col, row)];
}

std::size_t Canvas::index(unsigned int col, unsigned int row) const noexcept {
  return static_cast<std::size_t>(row) * this->renderingWidth_ + col;
}

} // namespace temprenderer::renderer
