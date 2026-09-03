#pragma once
#include "core/logging/LoggerManager.hpp"
#include "core/math/Color.hpp"
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
inline math::Color parseColorDataFromConfig(const toml::table &table) {
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
  return color;
}
} // namespace temprenderer::core::config
