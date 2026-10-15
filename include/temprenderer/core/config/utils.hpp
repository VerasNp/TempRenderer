#pragma once
#include "RenderConfig.hpp"
#include "SceneConfig.hpp"
#include "core/logging/LoggerManager.hpp"
#include "core/math/Materials.hpp"
#include "kwp/Point3.hpp"

namespace temprenderer::core::config {

/**
 * @brief TODO
 *
 * @param point3DData
 * @return
 */
inline kwp::Point3
parsePoint3DDataFromConfig(const parsers::ConfigValue &point3DData) {
  const float xNode = point3DData.get("x")->asFloat();
  const float yNode = point3DData.get("y")->asFloat();
  const float zNode = point3DData.get("z")->asFloat();
  return kwp::Point3{xNode, yNode, zNode};
}

/**
 * @brief TODO
 *
 * @param vector3DData
 * @return
 */
inline kwp::Vec3
parseVector3DDataFromConfig(const parsers::ConfigValue &vector3DData) {
  const float xNode = vector3DData.get("x")->asFloat();
  const float yNode = vector3DData.get("y")->asFloat();
  const float zNode = vector3DData.get("z")->asFloat();
  return kwp::Vec3{xNode, yNode, zNode};
}

/**
 * @brief TODO
 *
 * @param colorData
 * @return
 */
inline math::ColorF
parseColorDataFromConfig(const parsers::ConfigValue &colorData) {
  const std::uint8_t rNode = colorData.get("r")->asInt<std::uint8_t>();
  const std::uint8_t gNode = colorData.get("g")->asInt<std::uint8_t>();
  const std::uint8_t bNode = colorData.get("b")->asInt<std::uint8_t>();
  return math::Color{.r = rNode, .g = gNode, .b = bNode}.toFloat();
}

inline std::shared_ptr<const math::Texture>
parseTextureFromConfig(const parsers::ConfigValue &textureConfig) {
  if (!textureConfig.isTable()) {
    return std::make_shared<const math::SolidColorTexture>(
        math::ColorF{1.0F, 1.0F, 1.0F});
  }

  const auto type = textureConfig.get("type");
  if (type && type->asString() == "solid") {
    const auto colorConfig = textureConfig.get("color");
    const auto color = colorConfig ? parseColorDataFromConfig(*colorConfig)
                                   : math::ColorF{1.0F, 1.0F, 1.0F};
    return std::make_shared<const math::SolidColorTexture>(color);
  }

  if (type && type->asString() == "wood") {
    const auto baseConfig = textureConfig.get("base_color");
    const auto ringConfig = textureConfig.get("ring_color");
    const auto baseColor = baseConfig ? parseColorDataFromConfig(*baseConfig)
                                      : math::ColorF{0.72F, 0.46F, 0.22F};
    const auto ringColor = ringConfig ? parseColorDataFromConfig(*ringConfig)
                                      : math::ColorF{0.38F, 0.20F, 0.08F};
    const float scale = textureConfig.get("scale")
                            ? textureConfig.get("scale")->asFloat(8.0F)
                            : 8.0F;
    const float turbulence =
        textureConfig.get("turbulence")
            ? textureConfig.get("turbulence")->asFloat(0.65F)
            : 0.65F;
    return std::make_shared<const math::WoodTexture>(baseColor, ringColor,
                                                     scale, turbulence);
  }

  return std::make_shared<const math::SolidColorTexture>(
      math::ColorF{1.0F, 1.0F, 1.0F});
}

/**
 * @brief Calculates physical width size
 *
 * @param resolutionWidth Number of columns of pixels on canvas
 * @param resolutionHeight Number of lines of pixels on canvas
 * @param viewportHeight Physical height size of viewport
 * @return
 */
[[nodiscard]] float inline calculateViewportWidth(
    const std::uint16_t resolutionWidth, const std::uint16_t resolutionHeight,
    const float viewportHeight) noexcept {
  const auto resolutionRatio = static_cast<double>(resolutionWidth) /
                               static_cast<double>(resolutionHeight);
  return viewportHeight * resolutionRatio;
}

/**
  * @brief Transform the aspect ratio string and set to it correspondent enum
  *
  * @param aspectRatio Aspec ratio string on form <integer>:<integer>
  * @return Correspondent enum to the string
  */
[[nodiscard]] inline AspectRatio
stringToAspectRatio(const std::string &aspectRatio) {
  if (aspectRatio == "16:9") {
    return AspectRatio::WIDESCREEN;
  }
  if (aspectRatio == "4:3") {
    return AspectRatio::STANDARD;
  }
  if (aspectRatio == "21:9") {
    return AspectRatio::ULTRAWIDE;
  }
  LC_LOG(logging::LogLevel::WARNING, "Unknown aspect_ratio: '" + aspectRatio +
                                         "', using WIDESCREEN as default");
  return AspectRatio::WIDESCREEN;
}

/**
 * @brief Transform enum to number of ratio
 *
 * @param aspectRatio Enum of aspect ratio
 * @return Ratio between width and height of canvas
 */
[[nodiscard]] inline float aspectRatioToScalar(const AspectRatio &aspectRatio) {
  if (aspectRatio == AspectRatio::WIDESCREEN) {
    return 16.0F / 9.0F;
  }
  if (aspectRatio == AspectRatio::STANDARD) {
    return 4.0F / 3.0F;
  }
  return 21.0F / 9.0F;
}

/**
  * @brief Calculates resolution height given aspect ratio and the resolution width
  *
  * @param aspectRatio Enum of aspect ratio
  * @param resolutionWidth Number of columns of pixels on canvas
  * @return Number of lines of pixels on canvas
  */
[[nodiscard]] std::uint16_t inline calculateResolutionHeight(
    const AspectRatio aspectRatio,
    const std::uint16_t resolutionWidth) noexcept {
  const float ratio = aspectRatioToScalar(aspectRatio);
  const std::uint16_t height = resolutionWidth / ratio;
  return (height < 1) ? 1 : height;
}

/**
 * @brief TODO
 *
 * @param objectType
 * @return
 */
[[nodiscard]] std::string inline objectTypeToString(
    const ObjectType objectType) noexcept {
  if (objectType == ObjectType::SPHERE) {
    return "sphere";
  }
  if (objectType == ObjectType::CONE) {
    return "cone";
  }
  if (objectType == ObjectType::PLANE) {
    return "plane";
  }
  if (objectType == ObjectType::CYLINDER) {
    return "cylinder";
  }
  return "unknown";
}

[[nodiscard]] std::string inline lightTypeToString(
    const LightType lightType) noexcept {
  if (lightType == LightType::POINT) {
    return "point";
  }
  if (lightType == LightType::AMBIENT) {
    return "ambient";
  }
  return "unknown";
}

/**
 * @brief TODO
 *
 * @tparam T
 * @param value
 * @param fallback
 * @param fieldName
 * @return
 */
template <typename T>
T parseOrDefault(std::optional<T> value, T fallback,
                 const std::string &fieldName) {
  if (value) {
    return *value;
  }
  LC_LOG_VERBOSE(logging::LogLevel::WARNING,
                 "Invalid '" + fieldName + "'field, using the default value");
  return fallback;
}

} // namespace temprenderer::core::config
