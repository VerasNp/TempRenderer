#pragma once
#include "RenderConfig.hpp"
#include "SceneConfig.hpp"
#include "core/logging/LoggerManager.hpp"
#include "core/math/Materials.hpp"
#include "kwp/Point3.hpp"
#include "toml++/impl/table.hpp"

namespace temprenderer::core::config {
/**
 * @brief TODO
 *
 * @param table
 * @return
 */
inline kwp::Point3 parsePoint3DDataFromConfig(const toml::table &table) {
  auto position = kwp::Point3();
  auto *const xNode = table.get_as<double>("x");
  auto *const yNode = table.get_as<double>("y");
  auto *const zNode = table.get_as<double>("z");
  if (xNode == nullptr || yNode == nullptr || zNode == nullptr) {
    LC_LOG(logging::LogLevel::WARNING,
           "position: 'x', 'y' or 'z' missing or not a float, "
           "keeping default eye position");
  } else {
    position = kwp::Point3{static_cast<kwp::Scalar>(xNode->get()),
                           static_cast<kwp::Scalar>(yNode->get()),
                           static_cast<kwp::Scalar>(zNode->get())};
  }
  return position;
}

/**
 * @brief TODO
 *
 * @param table
 * @return
 */
inline math::ColorF parseColorDataFromConfig(const toml::table &table) {
  auto color = math::Color();
  auto *const rNode = table.get_as<std::int64_t>("r");
  auto *const gNode = table.get_as<std::int64_t>("g");
  auto *const bNode = table.get_as<std::int64_t>("b");
  if (rNode == nullptr || gNode == nullptr || bNode == nullptr) {
    LC_LOG(logging::LogLevel::ERROR,
           "rgb: 'r', 'g' or 'b' missing or not a float, "
           "keeping default eye position");
  } else {
    color = math::Color{.r = static_cast<std::uint8_t>(rNode->get()),
                        .g = static_cast<std::uint8_t>(gNode->get()),
                        .b = static_cast<std::uint8_t>(bNode->get())};
  }
  return color.toFloat();
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
[[nodiscard]] std::optional<std::string> inline objectTypeToString(
    const ObjectType objectType) noexcept {
  if (objectType == ObjectType::SPHERE) {
    return "sphere";
  }
  return std::nullopt;
}

/**
 * @brief TODO
 *
 * @param objectType
 * @return
 */
[[nodiscard]] std::optional<ObjectType> inline stringToObjectType(
    const std::string &objectType) noexcept {
  if (objectType == "sphere") {
    return ObjectType::SPHERE;
  }
  return std::nullopt;
}

/**
 * @brief TODO
 *
 * @param lightType
 * @return
 */
[[nodiscard]] std::optional<LightType> inline stringToLightType(
    const std::string &lightType) noexcept {
  if (lightType == "point") {
    return LightType::POINT;
  }
  if (lightType == "ambient") {
    return LightType::AMBIENT;
  }
  return std::nullopt;
}

template <typename T>
T parseOrDefault(std::optional<T> value, T fallback,
                 const std::string &fieldName) {
  if (value) {
    return *value;
  }
  LC_LOG_VERBOSE(logging::LogLevel::WARNING,
                 "Campo '{}' inválido, usando padrão");
  LC_LOG(logging::LogLevel::WARNING, "Unknown aspect_ratio: '" + aspectRatio +
                                         "', using WIDESCREEN as default");
  return fallback;
}

} // namespace temprenderer::core::config
