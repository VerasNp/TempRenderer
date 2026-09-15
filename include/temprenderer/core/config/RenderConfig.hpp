#pragma once

#include "core/parsers/ParserPort.hpp"

#include <cstdint>

namespace temprenderer::core::config {
/**
 * Possible aspect ratio values
 */
enum class AspectRatio { WIDESCREEN, STANDARD, ULTRAWIDE };

/**
 * @brief Render configs
 */
struct RenderConfig {
  /**
   * Number of columns of pixels on canvas
   */
  std::uint16_t resolutionWidth = 1280;
  /**
   * Number of lines of pixels on canvas
   */
  std::uint16_t resolutionHeight = 720;
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
  float viewportWidth = 3.55555558F;

  /**
   * @brief Loads render config into struct data structure
   *
   * @param renderConfig TODO
   * @return Render config struct
   */
  [[nodiscard]] static RenderConfig
  loadRenderConfig(const parsers::ConfigValue &renderConfig);
};
} // namespace temprenderer::core::config
