#include "temprenderer/renderer/RendererManager.hpp"
#include "temprenderer/core/config/EngineConfig.hpp"
#include "temprenderer/core/logging/LoggerManager.hpp"
#include "temprenderer/platform/graphics/WindowManager.hpp"
#include <cassert>

namespace temprenderer::renderer {

void RendererManager::startUp() {
  if (isRendererInit_) {
    return;
  }
  assert(
      configSet_ &&
      "RendererManager: setEngineConfig() deve ser chamado antes de startUp()");
  LC_LOG(core::logging::LogLevel::INFO, "Starting renderer manager");
  this->window_.startUp();
  const platform::graphics::WindowProps props{
      this->config_.window.title,
      this->config_.window.width,
      this->config_.window.height,
  };

  if (!this->window_.createWindow(props)) {
    LC_LOG(core::logging::LogLevel::ERROR, "Failed to create window");
    isRendererInit_ = false;
    return;
  }

  isRendererInit_ = true;
}

void RendererManager::shutDown() {
  if (!isRendererInit_) {
    return;
  }
  this->window_.shutDown();
  isRendererInit_ = false;
}

void RendererManager::setEngineConfig(
    const core::config::EngineConfig &config) {
  this->config_ = config;
  configSet_ = true;
}

void RendererManager::run() const {
  while (!this->window_.shouldClose()) {
    this->window_.update();
  }
}

} // namespace temprenderer::renderer
