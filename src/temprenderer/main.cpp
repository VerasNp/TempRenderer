#include "core/config/SceneConfig.hpp"
#include "temprenderer/ApplicationManager.hpp"
#include "temprenderer/core/cli/CliOptions.hpp"
#include "temprenderer/core/logging/LoggerManager.hpp"

int main(int argc, char **argv) {
  temprenderer::core::logging::LoggerManager gLoggerManager;
  temprenderer::ApplicationManager gApplicationManager;
  gLoggerManager.startUp();
  auto [applicationConfigFilePath, sceneConfigFilePath, verbose] =
      temprenderer::core::cli::parse(argc, argv);
  temprenderer::core::logging::LoggerManager::setVerbose(verbose);
  const temprenderer::core::config::ApplicationConfig applicationConfig =
      temprenderer::core::config::ApplicationConfig::loadFromFile(
          applicationConfigFilePath);
  gApplicationManager.setApplicationConfig(applicationConfig);
  const temprenderer::core::config::SceneConfig sceneConfig =
      temprenderer::core::config::SceneConfig::loadFromFile(
          sceneConfigFilePath);
  // if (sceneConfig !== nullptr) {
  //   gApplicationManager.setSceneConfig(sceneConfig);
  // }
  gApplicationManager.startUp();
  gApplicationManager.run();
  gApplicationManager.shutDown();
  gLoggerManager.shutDown();
}
