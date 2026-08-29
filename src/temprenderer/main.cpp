#include "temprenderer/Application.hpp"
#include "temprenderer/core/cli/CliOptions.hpp"
#include "temprenderer/core/logging/LoggerManager.hpp"

temprenderer::core::logging::LoggerManager gLoggerManager;
temprenderer::Application gApplication;

int main(int argc, char **argv) {
  auto [configPath, verbose] = temprenderer::core::cli::parse(argc, argv);
  temprenderer::core::logging::LoggerManager::setVerbose(verbose);
  temprenderer::Application::setConfigFilePath(configPath);
  gLoggerManager.startUp();
  gApplication.startUp();
  gApplication.run();
  gApplication.shutDown();
  gLoggerManager.shutDown();
}
