#include "temprenderer/Application.hpp"
#include "temprenderer/core/logging/LoggerManager.hpp"
#include "temprenderer/renderer/Camera.hpp"
#include "temprenderer/renderer/RayCastIntegrator.hpp"
#include "temprenderer/renderer/Sphere.hpp"

namespace temprenderer {

void Application::startUp() {
  if (this->isApplicationInit_) {
    return;
  }
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

  /**
   * SEPARAR EM CONFIGURAÇÃO A PARTIR DAQUI
   */
  renderer::Camera camera{
      this->config_.camera.eye,
      this->config_.render.resolutionWidth,
      this->config_.render.resolutionHeight,
      this->config_.render.viewportWidth,
      this->config_.render.viewportHeight,
  };

  scene::Sphere sphere({0, 0, -10.F}, 1.0F);

  renderer::RayCastIntegrator integrator(
      camera, this->config_.render.resolutionWidth,
      this->config_.render.resolutionHeight, core::math::Color{255, 0, 0, 1},
      core::math::Color{100, 100, 100, 1});
  renderer::Canvas canvas = integrator.render(sphere);

  /**
   * ATE AQUI
   */
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

} // namespace temprenderer
