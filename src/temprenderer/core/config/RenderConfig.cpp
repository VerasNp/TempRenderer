#include "core/config/RenderConfig.hpp"

#include "core/logging/LoggerManager.hpp"
#include <kwp/kwp_config.hpp>
#include <string>

namespace temprenderer::core::config {
namespace {
/**
  * @brief Calculates resolution height given aspect ratio and the resolution width
  *
  * @param aspectRatio Enum of aspect ratio
  * @param resolutionWidth Number of columns of pixels on canvas
  * @return Number of lines of pixels on canvas
  */
[[nodiscard]] unsigned int
calculateResolutionHeight(const AspectRatio aspectRatio,
                          const unsigned int resolutionWidth) noexcept {
  const kwp::Scalar ratio = RenderConfig::aspectRatioToScalar(aspectRatio);
  const unsigned int height =
      static_cast<unsigned int>(resolutionWidth / ratio);
  return (height < 1) ? 1 : height;
}

/**
 * @brief Calculates physical width size
 *
 * @param resolutionWidth Number of columns of pixels on canvas
 * @param resolutionHeight Number of lines of pixels on canvas
 * @param viewportHeight Physical height size of viewport
 * @return
 */
[[nodiscard]] float
calculateViewportWidth(const unsigned int resolutionWidth,
                       const unsigned int resolutionHeight,
                       const float viewportHeight) noexcept {
  return viewportHeight *
         (static_cast<float>(resolutionWidth) / resolutionHeight);
}

/**
  * @brief Transform the aspect ratio string and set to it correspondent enum
  *
  * @param aspectRatio Aspec ratio string on form <integer>:<integer>
  * @return Correspondent enum to the string
  */
[[nodiscard]] AspectRatio stringToAspectRatio(const std::string &aspectRatio) {
  if (aspectRatio == "16:9") {
    return AspectRatio::WIDESCREEN;
  }
  if (aspectRatio == "4:3") {
    return AspectRatio::STANDARD;
  }
  if (aspectRatio == "21:9") {
    return AspectRatio::ULTRAWIDE;
  }
  LC_LOG(logging::LogLevel::WARNING, "aspect_ratio desconhecido: '" +
                                         aspectRatio +
                                         "', usando WIDESCREEN como padrao");
  return AspectRatio::WIDESCREEN;
}
} // namespace

/**
 * @brief Transform enum to number of ratio
 *
 * @param aspectRatio Enum of aspect ratio
 * @return Ratio between width and height of canvas
 */
[[nodiscard]] float
RenderConfig::aspectRatioToScalar(const AspectRatio &aspectRatio) {
  switch (aspectRatio) {
  case AspectRatio::WIDESCREEN:
    return 16.0F / 9.0F;
  case AspectRatio::STANDARD:
    return 4.0F / 3.0F;
  case AspectRatio::ULTRAWIDE:
    return 21.0F / 9.0F;
  default:
    return 16.0F / 9.0F;
  }
}

RenderConfig RenderConfig::loadRenderConfig(const toml::table &table) {
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Loading render config");
  RenderConfig config;
  config.resolutionWidth =
      table["resolution_width"].value_or(config.resolutionWidth);
  if (const auto aspectStr = table["aspect_ratio"].value<std::string>();
      !aspectStr) {
    config.aspectRatio = stringToAspectRatio(*aspectStr);
  }
  config.resolutionHeight =
      calculateResolutionHeight(config.aspectRatio, config.resolutionWidth);
  config.viewportHeight =
      table["viewport_height"].value_or(config.viewportHeight);
  config.viewportWidth = calculateViewportWidth(
      config.resolutionWidth, config.resolutionHeight, config.viewportHeight);
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Render config loaded successfully");
  return config;
}
} // namespace temprenderer::core::config
