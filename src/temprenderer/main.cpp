#include "temprenderer/ApplicationManager.hpp"
#include "temprenderer/core/cli/CliOptions.hpp"
#include "temprenderer/core/logging/LoggerManager.hpp"

temprenderer::core::logging::LoggerManager gLoggerManager;
temprenderer::ApplicationManager gApplicationManager;

int main(int argc, char **argv) {
  auto [configPath, verbose] = temprenderer::core::cli::parse(argc, argv);
  temprenderer::core::logging::LoggerManager::setVerbose(verbose);
  const temprenderer::core::config::ApplicationConfig applicationConfig =
      temprenderer::core::config::ApplicationConfigLoader::loadFromFile(
          configPath);
  gApplicationManager.setApplicationConfig(applicationConfig);
  gLoggerManager.startUp();
  gApplicationManager.startUp();
  gApplicationManager.run();
  gApplicationManager.shutDown();
  gLoggerManager.shutDown();
}
