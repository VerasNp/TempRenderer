#pragma once

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
  float viewportHeight = 2.0;
  float viewportWidth = 2.0;

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

struct ApplicationConfig {
  WindowConfig window;
  RenderConfig render;
};

class ApplicationConfigLoader {
public:
  static ApplicationConfig loadFromFile(const std::string &path);
};

} // namespace temprenderer::core::config
