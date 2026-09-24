#pragma once
#include "CameraConfig.hpp"
#include "LightConfig.hpp"
#include "ObjectConfig.hpp"
#include "WorldConfig.hpp"
#include "core/parsers/ParserPort.hpp"
#include <vector>

namespace temprenderer::core::config {

/**
 * @brief Scene configs
 */
struct SceneConfig {
  WorldConfig world;
  CameraConfig camera;
  std::vector<LightConfig> lights;
  std::vector<ObjectConfig> objects;

  /**
   * @brief TODO
   *
   * @param config
   * @return
   */
  static SceneConfig loadConfig(const parsers::ConfigValue &config);
};
} // namespace temprenderer::core::config
