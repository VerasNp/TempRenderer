#include "temprenderer/Application.hpp"

#include "temprenderer/core/debug/ApplicationDebug.hpp"
#include "temprenderer/core/logging/LoggerManager.hpp"
#include "temprenderer/renderer/Camera.hpp"
#include "temprenderer/renderer/RayCastIntegrator.hpp"
#include "temprenderer/renderer/Sphere.hpp"

namespace temprenderer {

void Application::startUp() {
  if (this->isApplicationInit_) {
    return;
  }
  const core::config::ApplicationConfig engineConfig =
      core::config::ApplicationConfigLoader::loadFromFile(configFilePath);
  this->config_ = engineConfig;
  LC_DUMP_DIE();
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
  // this->editorManager_.setWindowManager(this->windowManager_);
  // this->editorManager_.startUp();
  this->renderManager_.startUp();

  /**
   * SEPARAR EM CONFIGURAÇÃO A PARTIR DAQUI
   */
  renderer::Camera camera{
      this->config_.camera.eye,
      this->config_.render.resolutionWidth,
      this->config_.render.resolutionHeight,
      static_cast<unsigned int>(this->config_.render.viewportWidth),
      static_cast<unsigned int>(this->config_.render.viewportHeight),
  };
  scene::Sphere sphere({0, 0, 34}, 30.0F);
  renderer::RayCastIntegrator integrator(
      camera, this->config_.render.resolutionWidth,
      this->config_.render.resolutionHeight, core::math::Color{255, 0, 0},
      core::math::Color{100, 100, 100});
  renderer::Canvas canvas = integrator.render(sphere);
  this->renderManager_.setCanvas(canvas);
  /**
   * ATE AQUI
   */
  this->isApplicationInit_ = true;
}

void Application::shutDown() {
  if (!this->isApplicationInit_) {
    return;
  }
  // this->editorManager_.shutDown();
  this->windowManager_.shutDown();
  this->isApplicationInit_ = false;
}
void Application::setApplicationConfig(
    const core::config::ApplicationConfig &config) {
  this->config_ = config;
}

void Application::run() {
  while (!this->windowManager_.shouldClose()) {
    this->windowManager_.update();
    // this->editorManager_.beginFrame();
    // this->editorManager_.endFrame();
    this->renderManager_.draw();
  }
}

} // namespace temprenderer
