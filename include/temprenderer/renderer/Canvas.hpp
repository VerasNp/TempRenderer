#pragma once
#include "temprenderer/core/math/Materials.hpp"
#include <vector>

namespace temprenderer::renderer {
/**
 * @brief Stores the color of every pixel produced by a render pass.
 */
class Canvas {
public:
  Canvas(unsigned int renderingWidth, unsigned int renderingHeight) noexcept;
  ~Canvas() = default;

  /**
  * @brief Writes the color of a single pixel.
  *
  * @param col Column index
  * @param row Row index
  * @param color Color to store
  */
  void set(unsigned int col, unsigned int row,
           core::math::Color color) noexcept;
  /**
   * @brief Reads the color of a single pixel.
   *
  * @param col Column index
  * @param row Row index
   * @return Color stored
   */
  [[nodiscard]] core::math::Color get(unsigned int col,
                                      unsigned int row) const noexcept;
  /**
   * @brief Raw pointer to the underlying pixel buffer.
   *
   * @return Pointer to the first pixel.
   */
  [[nodiscard]] const core::math::Color *data() const noexcept {
    return pixels_.data();
  }
  /**
   *  @brief Number of columns (nCol) in the canvas
   *
   * @return Number of columns
   */
  [[nodiscard]] unsigned int getRenderingWidth() const noexcept {
    return this->renderingWidth_;
  }
  /**
 * @brief Number of rows (nLin) in the canvas.
 *
 * @return Number of rows
 */
  [[nodiscard]] unsigned int getRenderingHeight() const noexcept {
    return this->renderingHeight_;
  }

private:
  unsigned int renderingWidth_;
  unsigned int renderingHeight_;
  std::vector<core::math::Color> pixels_;

  [[nodiscard]] std::size_t index(unsigned int col,
                                  unsigned int row) const noexcept;
};
} // namespace temprenderer::renderer
