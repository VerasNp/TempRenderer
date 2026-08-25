#include "temprenderer/Application.hpp"
#include "temprenderer/core/logging/LoggerManager.hpp"

namespace temprenderer {

void Application::startUp() {
  if (this->isApplicationInit_) {
    return;
  }
  this->windowManager_.startUp();
  LC_LOG(core::logging::LogLevel::INFO, "Starting application");
  this->windowManager_.startUp();
  const platform::graphics::WindowProps windowsProps{
      this->config_.window.title,
      this->config_.window.width,
      this->config_.window.height,
  };
  if (!this->windowManager_.createWindow(windowsProps)) {
    LC_LOG(core::logging::LogLevel::ERROR, "Failed to create window");
    this->isApplicationInit_ = false;
    return;
  }
  this->editorManager_.setWindowManager(this->windowManager_);
  this->editorManager_.startUp();
  this->isApplicationInit_ = true;
}

void Application::shutDown() {
  if (!this->isApplicationInit_) {
    return;
  }
  this->editorManager_.shutDown();
  this->windowManager_.shutDown();
  this->isApplicationInit_ = false;
}

void Application::run() {
  while (!this->windowManager_.shouldClose()) {
    this->windowManager_.update();
    this->editorManager_.beginFrame();
    this->editorManager_.endFrame();
  }
}

void Application::setWindowManager(
    platform::graphics::WindowManager windowManager) {
  this->windowManager_ = windowManager;
}
} // namespace temprenderer
