#include "temprenderer/core/config/EngineConfig.hpp"
#include "temprenderer/core/logging/LoggerManager.hpp"
#include "temprenderer/renderer/RendererManager.hpp"

temprenderer::core::logging::LoggerManager gLoggerManager;
temprenderer::renderer::RendererManager gRendererManager;

int main() {
  gLoggerManager.startUp();
  const temprenderer::core::config::EngineConfig engineConfig =
      temprenderer::core::config::EngineConfigLoader::loadFromFile(
          "engine_config.toml");
  gRendererManager.setEngineConfig(engineConfig);
  gRendererManager.startUp();
  gRendererManager.shutDown();
  gLoggerManager.shutDown();
}
