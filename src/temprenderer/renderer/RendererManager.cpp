#include "temprenderer/renderer/RendererManager.hpp"
#include "temprenderer/core/config/EngineConfig.hpp"
#include "temprenderer/platform/graphics/WindowManager.hpp"

#include <iostream>

namespace temprenderer::renderer {
void RendererManager::startUp() {
  this->window_.startUp();
  const platform::graphics::WindowProps props{
      this->config_.window.title,
      this->config_.window.width,
      this->config_.window.height,
  };
  if (!this->window_.createWindow(props)) {
    exit(1);
  }
  while (!this->window_.shouldClose()) {
    this->window_.update();
  }
}

void RendererManager::shutDown() { this->window_.shutDown(); }

void RendererManager::setEngineConfig(
    const core::config::EngineConfig &config) {
  this->config_ = config;
}

} // namespace temprenderer::renderer
