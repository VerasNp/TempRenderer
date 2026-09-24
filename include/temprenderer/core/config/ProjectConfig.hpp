#pragma once
#include "SceneConfig.hpp"

#include <string>

namespace temprenderer::core::config {
struct ProjectConfig {
  std::string name = "";
  SceneConfig scene;

  /**
  * @brief Loads project config from file
  *
  * @param projectConfigFilePath Path to project config file
  * @return Project config
  */
  [[nodiscard]] static ProjectConfig
  loadFromFile(const std::string &projectConfigFilePath = "");
};
} // namespace temprenderer::core::config
