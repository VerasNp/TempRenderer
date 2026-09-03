#include "temprenderer/ApplicationManager.hpp"

#include "scene/SceneComposer.hpp"
#include "temprenderer/core/debug/ApplicationDebug.hpp"
#include "temprenderer/core/logging/LoggerManager.hpp"
#include "temprenderer/renderer/Camera.hpp"
#include "temprenderer/renderer/RayCastIntegrator.hpp"
#include "temprenderer/renderer/Sphere.hpp"

namespace temprenderer {

void ApplicationManager::startUp() {
  if (this->isApplicationInit_) {
    return;
  }
  LC_LOG(core::logging::LogLevel::INFO,
         "Starting up application manager, workers working...");
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
  this->renderManager_.startUp();
  this->scene_ = scene::SceneComposer::compose(this->config_.scene);
  this->camera_ = renderer::Camera{
      this->config_.camera.eye,
      this->config_.render.resolutionWidth,
      this->config_.render.resolutionHeight,
      this->config_.render.viewportWidth,
      this->config_.render.viewportHeight,
  };
  this->editorManager_.mainLayout().setOnRenderRequested([this]() {
    this->renderScene();
    this->editorManager_.renderResult().setTexture(
        this->renderManager_.getTextureId(),
        this->config_.render.resolutionWidth,
        this->config_.render.resolutionHeight);
    this->editorManager_.renderResult().open();
  });
  this->isApplicationInit_ = true;
}

void ApplicationManager::shutDown() {
  if (!this->isApplicationInit_) {
    return;
  }
  LC_LOG_VERBOSE(core::logging::LogLevel::INFO,
                 "Begin to shut down the application");
  this->renderManager_.shutDown();
  this->editorManager_.shutDown();
  this->windowManager_.shutDown();
  this->isApplicationInit_ = false;
}
void ApplicationManager::setApplicationConfig(
    const core::config::ApplicationConfig &config) {
  this->config_ = config;
}
void ApplicationManager::renderScene() {
  renderer::RayCastIntegrator integrator(
      this->camera_.value(), this->config_.render.resolutionWidth,
      this->config_.render.resolutionHeight, core::math::Color(100, 100, 100));
  renderer::Canvas canvas = integrator.render(this->scene_);
  this->renderManager_.setCanvas(canvas);
}

void ApplicationManager::run() {
  while (!this->windowManager_.shouldClose()) {
    this->windowManager_.update();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    this->editorManager_.beginFrame();
    this->editorManager_.endFrame();
    this->windowManager_.swapBuffers();
  }
}

} // namespace temprenderer
