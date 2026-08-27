#pragma once

#include "kwp/Point3.hpp"
#include "kwp/Vec3.hpp"

#include <kwp/kwp_config.hpp>
#include <string>

namespace temprenderer::core::config {
enum class AspectRatio { WIDESCREEN, STANDARD, ULTRAWIDE };

struct WindowConfig {
  std::string title = "TempRenderer Engine";
  unsigned int width = 800;
  unsigned int height = 600;
};

struct RenderConfig {
  unsigned int resolutionWidth = 800;
  unsigned int resolutionHeight = 600;
  AspectRatio aspectRatio = AspectRatio::WIDESCREEN;
  float viewportHeight = 2.0F;
  float viewportWidth = 2.0F;

  [[nodiscard]] static float aspectRatioToScalar(AspectRatio aspectRatio);

  [[nodiscard]] static unsigned int
  calculateResolutionHeight(AspectRatio aspectRatio,
                            unsigned int resolutionWidth) noexcept;

  [[nodiscard]] static float
  calculateViewportWidth(unsigned int resolutionWidth,
                         unsigned int resolutionHeight,
                         float viewportHeight) noexcept;

  [[nodiscard]] static AspectRatio
  stringToAspectRatio(const std::string &aspectRatio);
};

struct CameraConfig {
  kwp::Point3 eye{0, 0, 0};
  kwp::Scalar focalLength = 1.0F;
};

struct ApplicationConfig {
  WindowConfig window;
  RenderConfig render;
  CameraConfig camera;
};

class ApplicationConfigLoader {
public:
  static ApplicationConfig loadFromFile(const std::string &path);
};

} // namespace temprenderer::core::config
