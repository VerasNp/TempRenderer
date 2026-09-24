#include "core/config/ProjectConfig.hpp"
#include "core/config/SceneConfig.hpp"
#include "temprenderer/ApplicationManager.hpp"
#include "temprenderer/core/cli/CliOptions.hpp"
#include "temprenderer/core/logging/LoggerManager.hpp"

int main(int argc, char **argv) {
  temprenderer::core::logging::LoggerManager gLoggerManager;
  temprenderer::ApplicationManager gApplicationManager;
  gLoggerManager.startUp();
  auto [applicationConfigFilePath, projectConfigFilePath, verbose] =
      temprenderer::core::cli::parse(argc, argv);
  temprenderer::core::logging::LoggerManager::setVerbose(verbose);
  const temprenderer::core::config::ApplicationConfig applicationConfig =
      temprenderer::core::config::ApplicationConfig::loadFromFile(
          applicationConfigFilePath);
  gApplicationManager.setApplicationConfig(applicationConfig);
  const temprenderer::core::config::ProjectConfig projectConfig =
      temprenderer::core::config::ProjectConfig::loadFromFile(
          projectConfigFilePath);
  gApplicationManager.setProjectConfig(projectConfig);
  gApplicationManager.startUp();
  gApplicationManager.run();
  gApplicationManager.shutDown();
  gLoggerManager.shutDown();
}
