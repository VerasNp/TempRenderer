#pragma once

#include "kwp/Point3.hpp"
#include "kwp/Vec3.hpp"

#include <kwp/kwp_config.hpp>
#include <string>

namespace temprenderer::core::config {
enum class AspectRatio { WIDESCREEN, STANDARD, ULTRAWIDE };

/**
 * @brief Window configs
 */
struct WindowConfig {
  /**
   * Window title
   */
  std::string title = "TempRenderer Engine";
  /**
   * Window width
   */
  unsigned int width = 800;
  /**
   * Window height
   */
  unsigned int height = 600;
};

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

  /**
   * @brief Transform enum to number of ratio
   *
   * @param aspectRatio Enum of aspect ratio
   * @return Ratio between width and height of canvas
   */
  [[nodiscard]] static float aspectRatioToScalar(AspectRatio aspectRatio);

  /**
   * @brief Calculates resolution height given aspect ratio and the resolution width
   *
   * @param aspectRatio Enum of aspect ratio
   * @param resolutionWidth Number of columns of pixels on canvas
   * @return Number of lines of pixels on canvas
   */
  [[nodiscard]] static unsigned int
  calculateResolutionHeight(AspectRatio aspectRatio,
                            unsigned int resolutionWidth) noexcept;

  /**
   * @brief Calculates physical width size
   *
   * @param resolutionWidth Number of columns of pixels on canvas
   * @param resolutionHeight Number of lines of pixels on canvas
   * @param viewportHeight Physical height size of viewport
   * @return
   */
  [[nodiscard]] static float
  calculateViewportWidth(unsigned int resolutionWidth,
                         unsigned int resolutionHeight,
                         float viewportHeight) noexcept;

  /**
   * @brief Transform the aspect ratio string and set to it correspondent enum
   *
   * @param aspectRatio Aspec ratio string on form <integer>:<integer>
   * @return Correspondent enum to the string
   */
  [[nodiscard]] static AspectRatio
  stringToAspectRatio(const std::string &aspectRatio);
};

/**
 * @brief Camera configs
 */
struct CameraConfig {
  /**
   * Position on camera
   */
  kwp::Point3 eye{0, 0, 0};
  /**
   * Distance from canvas
   */
  kwp::Scalar focalLength = 1.0F;
};

/**
 * @brief General application config
 */
struct ApplicationConfig {
  /**
   * Window configs
   */
  WindowConfig window;
  /**
   * Render configs
   */
  RenderConfig render;
  /**
   * Camera configs
   */
  CameraConfig camera;

  /**
   * @brief Loads application config from file
   *
   * @param path Path to config file
   * @return General application config
   */
  static ApplicationConfig loadFromFile(const std::string &path);
};
} // namespace temprenderer::core::config
