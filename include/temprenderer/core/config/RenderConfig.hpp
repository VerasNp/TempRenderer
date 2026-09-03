#pragma once

#include <toml++/toml.hpp>

namespace temprenderer::core::config {

enum class AspectRatio { WIDESCREEN, STANDARD, ULTRAWIDE };

/**
 * @brief Render configs
 */
struct RenderConfig {
  /**
   * Number of columns of pixels on canvas
   */
  unsigned int resolutionWidth = 800;
  /**
   * Number of lines of pixels on canvas
   */
  unsigned int resolutionHeight = 600;
  /**
   * Ratio between width and height
   */
  AspectRatio aspectRatio = AspectRatio::WIDESCREEN;
  /**
   * Physical height size of viewport
   */
  float viewportHeight = 2.0F;
  /**
   * Physical width size of viewport
   */
  float viewportWidth = 2.0F;

  [[nodiscard]] static RenderConfig loadRenderConfig(const toml::table &table);
  [[nodiscard]] static float
  aspectRatioToScalar(const AspectRatio &aspectRatio);
};
} // namespace temprenderer::core::config
