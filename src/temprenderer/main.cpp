#include "temprenderer/Application.hpp"
#include "temprenderer/core/config/ApplicationConfig.hpp"
#include "temprenderer/core/logging/LoggerManager.hpp"

temprenderer::core::logging::LoggerManager gLoggerManager;
temprenderer::Application gApplication;

int main() {
  const temprenderer::core::config::ApplicationConfig engineConfig =
      temprenderer::core::config::ApplicationConfigLoader::loadFromFile(
          "engine_config.toml");
  gLoggerManager.startUp();
  gApplication.startUp();
  gApplication.run();
  gApplication.shutDown();
  gLoggerManager.shutDown();
}
