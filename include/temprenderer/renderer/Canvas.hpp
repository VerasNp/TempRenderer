#pragma once
#include "temprenderer/core/config/ApplicationConfig.hpp"
#include "temprenderer/core/math/Color.hpp"

#include <kwp/kwp_config.hpp>
#include <vector>

namespace temprenderer::renderer {

class Canvas {
public:
  Canvas(unsigned int renderingWidth, unsigned int renderingHeight) noexcept;
  ~Canvas() = default;

  void set(unsigned int col, unsigned int row,
           core::math::Color color) noexcept;
  [[nodiscard]] core::math::Color get(unsigned int col,
                                      unsigned int row) const noexcept;
  [[nodiscard]] const core::math::Color *data() const noexcept {
    return pixels_.data();
  }
  [[nodiscard]] unsigned int getRenderingWidth() const noexcept {
    return this->renderingWidth_;
  }
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
