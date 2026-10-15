#include "core/config/ProjectConfig.hpp"

namespace temprenderer::core::config {
ProjectConfig
ProjectConfig::loadFromFile(const std::string &projectConfigFilePath) {
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Loading project config");
  if (projectConfigFilePath.empty()) {
    LC_LOG(logging::LogLevel::WARNING,
           "Project config file path is empty; using default project config");
    return ProjectConfig{};
  }
  const std::unique_ptr<parsers::ParserPort> jsonParser =
      std::make_unique<parsers::NlohmannJSONParserAdapter>();
  const parsers::ConfigValue rootProject =
      jsonParser->parserFile(projectConfigFilePath);
  ProjectConfig config;
  try {
    config.name = rootProject.get("name")->asString();
    if (const auto sceneConfig = rootProject.get("scene")) {
      config.scene = SceneConfig::loadConfig(*sceneConfig);
    }
  } catch (const std::exception &err) {
    LC_LOG(logging::LogLevel::ERROR,
           std::string("Unexpected error while loading project configs '") +
               projectConfigFilePath + "': " + err.what());
    throw;
  }
  LC_LOG_VERBOSE(logging::LogLevel::INFO, "Project config loaded successfully");
  return config;
}
} // namespace temprenderer::core::config
