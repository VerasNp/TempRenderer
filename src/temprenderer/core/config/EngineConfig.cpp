#include "temprenderer/core/config/EngineConfig.hpp"

#include "temprenderer/core/logging/LoggerManager.hpp"

#include <toml++/toml.hpp>

namespace temprenderer::core::config {
EngineConfig EngineConfigLoader::loadFromFile(const std::string &path) {
  toml::table table;
  try {
    LC_LOG(logging::LogLevel::INFO, "Loading config from " + path + "...");
    table = toml::parse_file(path);
  } catch (const toml::parse_error &err) {
    LC_LOG(logging::LogLevel::ERROR, "Fail to parser log file");
    throw std::runtime_error(std::string("Erro no arquivo de config: ") +
                             err.description().data());
  }
  EngineConfig config;
  if (auto window = table["window"].as_table()) {
    config.window.title = (*window)["title"].value_or(config.window.title);
    config.window.width = (*window)["width"].value_or(config.window.width);
    config.window.height = (*window)["height"].value_or(config.window.height);
  }
  return config;
}
} // namespace temprenderer::core::config
