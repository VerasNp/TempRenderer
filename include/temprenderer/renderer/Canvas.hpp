#pragma once
#include "temprenderer/core/config/ApplicationConfig.hpp"
#include "temprenderer/core/math/Color.hpp"

#include <kwp/kwp_config.hpp>

namespace temprenderer::renderer {

class Canvas {
public:
  Canvas(unsigned int renderingWidth, unsigned int renderingHeight);
  ~Canvas();

  void set(unsigned int col, unsigned int row,
           core::math::Color color) noexcept;

private:
  unsigned int renderingWidth_;
  unsigned int renderingHeight_;
  kwp::Scalar aspectRatio_;
  core::config::ApplicationConfig applicationConfig_;
};
} // namespace temprenderer::renderer
